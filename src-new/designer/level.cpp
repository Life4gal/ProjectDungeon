// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/level.hpp>

#include <algorithm>
#include <array>
#include <vector>
#include <span>
#include <optional>

#include <designer/room.hpp>

#include <manager/random.hpp>

#include <prometheus/platform/os.hpp>
#include <SFML/System/Vector2.hpp>
#include <spdlog/spdlog.h>

namespace pd::designer
{
	namespace
	{
		using size_type = Level::size_type;
		using position_type = Level::position_type;

		class Layout final
		{
		public:
			using rooms_type = std::unordered_map<position_type, blueprint::RoomType>;

			// 如果无法满足BOSS房间+出口房间组合,重新生成前的最大尝试次数
			constexpr static size_type max_regeneration_attempts = 10;

			size_type width;
			size_type height;

			rooms_type rooms;

			[[nodiscard]] auto is_within_bounds(const position_type position) const noexcept -> bool
			{
				return position.x < width and position.y < height;
			}

			[[nodiscard]] auto has_room(const position_type position) const noexcept -> bool
			{
				PROMETHEUS_PLATFORM_ASSUME(is_within_bounds(position));

				return rooms.contains(position);
			}

			[[nodiscard]] auto type_of(const position_type position) const noexcept -> blueprint::RoomType
			{
				PROMETHEUS_PLATFORM_ASSUME(is_within_bounds(position) and has_room(position));

				return rooms.at(position);
			}

			auto set_type(const position_type position, const blueprint::RoomType type) noexcept -> void
			{
				PROMETHEUS_PLATFORM_ASSUME(is_within_bounds(position));

				rooms[position] = type;
			}

			[[nodiscard]] auto get_unplaced_neighbors(const position_type position) const noexcept -> std::vector<position_type>
			{
				std::vector<position_type> neighbors{};
				for (const auto [x, y]: RoomNavigation::directions)
				{
					const position_type next_position{.x = position.x + x, .y = position.y + y};

					if (not is_within_bounds(next_position))
					{
						continue;
					}

					if (not has_room(position))
					{
						continue;
					}

					neighbors.push_back(next_position);
				}

				return neighbors;
			}

			[[nodiscard]] auto get_unplaced_neighbors(const std::span<const position_type> positions) const noexcept -> std::vector<position_type>
			{
				std::vector<position_type> all_neighbors{};

				// 获取
				for (const auto& position: positions)
				{
					auto neighbors = get_unplaced_neighbors(position);
					all_neighbors.append_range(std::move(neighbors));
				}
				// 去重
				{
					std::ranges::sort(
						all_neighbors,
						[](const position_type& lhs, const position_type& rhs) noexcept -> bool
						{
							return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
						}
					);

					const auto r = std::ranges::unique(all_neighbors);
					all_neighbors.erase(r.begin(), all_neighbors.end());
				}

				return all_neighbors;
			}

			[[nodiscard]] auto has_placed_neighbor(const position_type position, const blueprint::RoomConnection neighbor) const noexcept -> bool
			{
				const auto direction = RoomNavigation::direction_of(neighbor);
				const position_type next_position{.x = position.x + direction.x, .y = position.y + direction.y};

				if (not is_within_bounds(next_position))
				{
					return false;
				}

				return has_room(next_position);
			}

			[[nodiscard]] auto count_placed_neighbors(const position_type position) const noexcept -> size_type
			{
				size_type count = 0;
				for (const auto [x, y]: RoomNavigation::directions)
				{
					const position_type next_position{.x = position.x + x, .y = position.y + y};

					if (not is_within_bounds(next_position))
					{
						continue;
					}

					if (not has_room(next_position))
					{
						continue;
					}

					count += 1;
				}

				return count;
			}

			[[nodiscard]] auto get_placed_neighbors_mask(const position_type position) const noexcept -> std::underlying_type_t<blueprint::RoomConnection>
			{
				std::underlying_type_t<blueprint::RoomConnection> mask = std::to_underlying(blueprint::RoomConnection::NONE);

				if (has_placed_neighbor(position, blueprint::RoomConnection::NORTH))
				{
					mask |= std::to_underlying(blueprint::RoomConnection::NORTH);
				}
				if (has_placed_neighbor(position, blueprint::RoomConnection::SOUTH))
				{
					mask |= std::to_underlying(blueprint::RoomConnection::SOUTH);
				}
				if (has_placed_neighbor(position, blueprint::RoomConnection::WEST))
				{
					mask |= std::to_underlying(blueprint::RoomConnection::WEST);
				}
				if (has_placed_neighbor(position, blueprint::RoomConnection::EAST))
				{
					mask |= std::to_underlying(blueprint::RoomConnection::EAST);
				}

				return mask;
			}

			[[nodiscard]] static auto generate(
				const size_type horizontal_count,
				const size_type vertical_count,
				const size_type count,
				const size_type start_x,
				const size_type start_y
			) noexcept -> std::optional<Layout>
			{
				// 主区域大小 = 总房间数 - BOSS - EXIT
				const auto main_region_count = count - 2;

				for (std::uint32_t attempt = 0; attempt < max_regeneration_attempts; ++attempt)
				{
					SPDLOG_INFO("Dungeon generation attempt {}/{}...", attempt + 1, max_regeneration_attempts);

					Layout layout{.width = horizontal_count, .height = vertical_count, .rooms = {}};
					std::vector<position_type> placed{};

					// ============================================================
					//  Step 1 — 放置起始房间
					// ============================================================

					layout.set_type({.x = start_x, .y = start_y}, blueprint::RoomType::START);
					placed.emplace_back(start_x, start_y);

					// ============================================================
					//  Step 2 — 随机前沿扩展,生长主区域,保证所有房间与 START 连通
					// ============================================================

					while (placed.size() < main_region_count)
					{
						auto neighbors = layout.get_unplaced_neighbors(placed);
						if (neighbors.empty())
						{
							break;
						}

						const auto chosen = manager::Random::int_inclusive(std::size_t{0}, neighbors.size() - 1);
						const auto position = neighbors[chosen];

						layout.set_type(position, blueprint::RoomType::STANDARD);
						placed.push_back(position);
					}

					// ============================================================
					//  Step 3 — 寻找 BOSS + EXIT 的合法放置位置
					//
					//  约束:
					//    · BOSS 所在空位恰好与 1 个主区域房间相邻 → 仅有 1 扇入口门
					//    · EXIT 所在空位仅与 BOSS 相邻, 不与任何主区域房间相邻
					//    → BOSS 恰好有 2 扇门(入口 + 通往EXIT)
					//    → EXIT 恰好有 2 扇门(通往BOSS + 出口)
					// ============================================================

					{
						class Candidate
						{
						public:
							position_type boss;
							position_type exit;
							// BOSS房间与起始房间的曼哈顿距离
							size_type manhattan_distance;
						};

						std::vector<Candidate> candidates{};

						for (size_type position_y = 0; position_y < vertical_count; ++position_y)
						{
							for (size_type position_x = 0; position_x < horizontal_count; ++position_x)
							{
								const position_type boss_position{.x = position_x, .y = position_y};

								if (not layout.has_room(boss_position))
								{
									continue;
								}

								// BOSS 候选: 恰好与 1 个已放置房间相邻
								if (layout.count_placed_neighbors(boss_position) != 1)
								{
									continue;
								}

								// 在 BOSS 候选四周寻找 EXIT 位置
								for (const auto [x, y]: RoomNavigation::directions)
								{
									const position_type exit_position{.x = boss_position.x + x, .y = boss_position.y + y};

									if (not layout.is_within_bounds(exit_position))
									{
										continue;
									}

									if (layout.has_room(exit_position))
									{
										continue;
									}

									// EXIT 除了 BOSS 之外不能有任何已放置的邻居
									bool valid = true;
									for (const auto [next_x, next_y]: RoomNavigation::directions)
									{
										const position_type exit_neighbor_position{.x = exit_position.x + next_x, .y = exit_position.y + next_y};

										// BOSS房间除外
										if (exit_neighbor_position == boss_position)
										{
											continue;
										}

										// 出界的邻居不算
										if (not layout.is_within_bounds(exit_neighbor_position))
										{
											continue;
										}

										// 只要有一个已放置的邻居,就不合法
										if (not layout.has_room(exit_neighbor_position))
										{
											continue;
										}

										valid = false;
										break;
									}

									if (valid)
									{
										const auto diff_x = static_cast<std::make_signed_t<size_type>>(boss_position.x) - static_cast<std::make_signed_t<size_type>>(start_x);
										const auto diff_y = static_cast<std::make_signed_t<size_type>>(boss_position.y) - static_cast<std::make_signed_t<size_type>>(start_y);

										const auto distance = std::abs(diff_x) + std::abs(diff_y);
										candidates.emplace_back(boss_position, exit_position, static_cast<size_type>(distance));
									}
								}
							}
						}

						if (candidates.empty())
						{
							// 无合法位置
							continue;
						}

						// // 优先选择距 START 较远的位置
						// std::ranges::sort(candidates, std::ranges::greater{}, &Candidate::manhattan_distance);

						const auto chosen = manager::Random::int_inclusive(std::size_t{0}, candidates.size() - 1);
						const auto [boss_position, exit_position, _] = candidates[chosen];

						layout.set_type(boss_position, blueprint::RoomType::BOSS);
						layout.set_type(exit_position, blueprint::RoomType::EXIT);
					}

					// ============================================================
					//  Step 4 — 从 STANDARD 房间中分配特殊类型
					//
					//  · KEY: 必须有 1 个
					//  · MERCHANT: 最多 1 个 (剩余 ≥ 2 个 STANDARD 时分配)
					//  · BONUS: 最多 1 个 (剩余 ≥ 2 个 STANDARD 时分配)
					//  → 始终保留至少 1 个 STANDARD 房间
					// ============================================================

					{
						std::vector<position_type> standards{};
						std::ranges::copy_if(
							placed,
							std::back_inserter(standards),
							[&layout](const position_type& position) noexcept -> bool
							{
								return layout.has_room(position) and layout.type_of(position) == blueprint::RoomType::STANDARD;
							}
						);

						if (standards.empty())
						{
							// 至少需要一个房间用于Key
							continue;
						}

						std::ranges::shuffle(standards, manager::Random::engine());

						// KEY
						{
							const auto key_position = standards.back();

							layout.set_type(key_position, blueprint::RoomType::KEY);
							standards.pop_back();
						}

						// MERCHANT(可选,保留至少 1 个STANDARD)
						if (standards.size() >= 2)
						{
							const auto merchant_position = standards.back();

							layout.set_type(merchant_position, blueprint::RoomType::MERCHANT);
							standards.pop_back();
						}

						// BONUS(可选,保留至少 1 个STANDARD)
						if (standards.size() >= 2)
						{
							const auto bonus_position = standards.back();

							layout.set_type(bonus_position, blueprint::RoomType::BONUS);
							standards.pop_back();
						}
					}

					constexpr auto char_of = [](const std::optional<blueprint::RoomType> type) noexcept -> char
					{
						if (not type.has_value())
						{
							return '.';
						}

						using namespace blueprint;
						switch (*type)
						{
							case RoomType::START:
							{
								return 'S';
							}
							case RoomType::STANDARD:
							{
								return '#';
							}
							case RoomType::BOSS:
							{
								return 'B';
							}
							case RoomType::KEY:
							{
								return 'K';
							}
							case RoomType::MERCHANT:
							{
								return 'M';
							}
							case RoomType::BONUS:
							{
								return 'R';
							}
							case RoomType::EXIT:
							{
								return 'E';
							}
							default: // NOLINT(clang-diagnostic-covered-switch-default)
							{
								return '?';
							}
						}
					};

					SPDLOG_INFO("----- DUNGEON LAYOUT ----- ");
					SPDLOG_INFO(
						"-NONE={} -START={} -STANDARD={} -BOSS={} -KEY={} -MERCHANT={} -BONUS={} -EXIT={}",
						char_of(std::nullopt),
						char_of(blueprint::RoomType::START),
						char_of(blueprint::RoomType::STANDARD),
						char_of(blueprint::RoomType::BOSS),
						char_of(blueprint::RoomType::KEY),
						char_of(blueprint::RoomType::MERCHANT),
						char_of(blueprint::RoomType::BONUS),
						char_of(blueprint::RoomType::EXIT)
					);

					// 第一行
					{
						std::string row{};
						// 每个元素占3格 + 前置3格空白
						row.reserve(horizontal_count * 2 + 3);

						// 空白
						row.append("   ");

						for (size_type x = 0; x < horizontal_count; ++x)
						{
							std::format_to(std::back_inserter(row), "{:>2}", x);
						}

						SPDLOG_INFO("{}", row);
					}

					for (size_type y = 0; y < vertical_count; ++y)
					{
						std::string row{};
						// 每个元素占3格 + 前置3格显示行号
						row.reserve(horizontal_count * 3 + 3);

						// 行号
						std::format_to(std::back_inserter(row), "{:>2} ", y);

						for (size_type x = 0; x < horizontal_count; ++x)
						{
							const position_type position{.x = x, .y = y};

							if (not layout.has_room(position))
							{
								std::format_to(std::back_inserter(row), "{:>2}", char_of(std::nullopt));
							}
							else
							{
								std::format_to(std::back_inserter(row), "{:>2}", char_of(layout.type_of(position)));
							}
						}

						SPDLOG_INFO("{}", row);
					}
					SPDLOG_INFO("----- DUNGEON LAYOUT ----- ");

					return layout;
				}

				// PROMETHEUS_PLATFORM_UNREACHABLE("Failed to generate dungeon layout after maximum attempts");
				SPDLOG_ERROR("Failed to generate dungeon layout after maximum attempts");
				return std::nullopt;
			}
		};
	}

	auto Level::generate(size_type horizontal_count, size_type vertical_count, size_type count, size_type start_x, size_type start_y) noexcept -> blueprint::Level
	{
		horizontal_count = std::ranges::clamp(horizontal_count, horizontal_min_count, horizontal_max_count);
		vertical_count = std::ranges::clamp(vertical_count, vertical_min_count, vertical_max_count);
		count = std::ranges::clamp(count, base_count, horizontal_count * vertical_count);
		start_x = std::ranges::min(start_x, horizontal_count - 1);
		start_y = std::ranges::min(start_y, vertical_count - 1);

		// 生成布局
		const auto layout_opt = Layout::generate(horizontal_count, vertical_count, count, start_x, start_y);
		if (not layout_opt.has_value())
		{
			// TODO: 错误处理
			PROMETHEUS_PLATFORM_UNREACHABLE("Failed to generate dungeon layout");
		}
		const auto& layout = *layout_opt;

		blueprint::Level level{.rooms = {}, .start_position = {.x = start_x, .y = start_y}};
		level.rooms.reserve(count);

		// 生成房间
		for (size_type y = 0; y < vertical_count; ++y)
		{
			for (size_type x = 0; x < horizontal_count; ++x)
			{
				const position_type position{.x = x, .y = y};

				if (not layout.has_room(position))
				{
					continue;
				}

				const auto mask = layout.get_placed_neighbors_mask(position);
				const auto type = layout.type_of(position);

				auto room = [&] noexcept -> blueprint::Room
				{
					switch (type)
					{
						case blueprint::RoomType::START:
						{
							// TODO
							return Room::standard(x, y, mask);
						}
						case blueprint::RoomType::STANDARD:
						{
							return Room::standard(x, y, mask);
						}
						case blueprint::RoomType::BOSS:
						{
							// TODO
							return Room::standard(x, y, mask);
						}
						case blueprint::RoomType::KEY:
						{
							// TODO
							return Room::standard(x, y, mask);
						}
						case blueprint::RoomType::MERCHANT:
						{
							// TODO
							return Room::standard(x, y, mask);
						}
						case blueprint::RoomType::BONUS:
						{
							// TODO
							return Room::standard(x, y, mask);
						}
						case blueprint::RoomType::EXIT:
						{
							// TODO
							return Room::standard(x, y, mask);
						}
						default: // NOLINT(clang-diagnostic-covered-switch-default)
						{
							PROMETHEUS_PLATFORM_UNREACHABLE();
						}
					}
				}();

				level.rooms.emplace(position, std::move(room));
			}
		}

		return level;
	}
}
