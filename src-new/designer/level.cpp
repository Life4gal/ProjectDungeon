// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/level.hpp>

#include <algorithm>
#include <array>
#include <vector>
#include <mdspan>
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
		using position_type = sf::Vector2<size_type>;

		using matrix_memory_type = std::unique_ptr<std::optional<blueprint::RoomType>[]>;
		using matrix_type = std::mdspan<std::optional<blueprint::RoomType>, std::extents<size_type, std::dynamic_extent, std::dynamic_extent>, std::layout_stride>;

		// 如果无法满足BOSS房间+出口房间组合,重新生成前的最大尝试次数
		constexpr size_type MaxRegenerationAttempts = 10;

		// 各个方向的坐标偏移
		constexpr std::array<sf::Vector2<std::make_signed_t<size_type>>, 4> Directions
		{{
				// NORTH
				{0, -1},
				// SOUTH
				{0, 1},
				// WEST
				{-1, 0},
				// EAST
				{1, 0}
		}};

		[[nodiscard]] auto make_matrix(const matrix_memory_type& memory, const size_type horizontal_count, const size_type vertical_count) noexcept -> matrix_type
		{
			return matrix_type
			{
					memory.get(),
					matrix_type::mapping_type
					{
							std::dextents<size_type, 2>{vertical_count, horizontal_count},
							std::array<size_type, 2>{horizontal_count, 1}
					}
			};
		}

		[[nodiscard]] auto is_within_bounds(const matrix_type& matrix, const position_type position) noexcept -> bool
		{
			const auto width = matrix.extent(1);
			const auto height = matrix.extent(0);

			return position.x < width and position.y < height;
		}

		[[nodiscard]] auto element_of(const matrix_type& matrix, const position_type position) noexcept -> std::optional<blueprint::RoomType>&
		{
			return matrix[position.y, position.x];
		}

		[[nodiscard]] auto get_unplaced_neighbors(const matrix_type& matrix, const position_type position) noexcept -> std::vector<position_type>
		{
			std::vector<position_type> neighbors{};
			for (const auto [x, y]: Directions)
			{
				const position_type next_position{position.x + x, position.y + y};

				if (not is_within_bounds(matrix, next_position))
				{
					continue;
				}

				if (element_of(matrix, next_position).has_value())
				{
					continue;
				}

				neighbors.push_back(next_position);
			}

			return neighbors;
		}

		[[nodiscard]] auto get_unplaced_neighbors(const matrix_type& matrix, const std::span<const position_type> positions) noexcept -> std::vector<position_type>
		{
			std::vector<position_type> all_neighbors{};

			// 获取
			for (const auto& position: positions)
			{
				auto neighbors = get_unplaced_neighbors(matrix, position);
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

		[[nodiscard]] auto count_placed_neighbors(const matrix_type& matrix, const position_type position) noexcept -> size_type
		{
			size_type count = 0;
			for (const auto [x, y]: Directions)
			{
				const position_type next_position{position.x + x, position.y + y};

				if (not is_within_bounds(matrix, next_position))
				{
					continue;
				}

				if (not element_of(matrix, next_position).has_value())
				{
					continue;
				}

				count += 1;
			}

			return count;
		}

		template<typename DoClean>
		auto generate_layout(
			const size_type horizontal_count,
			const size_type vertical_count,
			const size_type count,
			const size_type start_x,
			const size_type start_y,
			matrix_type& matrix,
			DoClean clean_matrix
		) noexcept -> bool //
			requires requires { clean_matrix(); }
		{
			// 主区域大小 = 总房间数 - BOSS - EXIT
			const auto main_region_count = count - 2;

			for (std::uint32_t attempt = 0; attempt < MaxRegenerationAttempts; ++attempt)
			{
				SPDLOG_INFO("Dungeon generation attempt {}/{}...", attempt + 1, MaxRegenerationAttempts);

				clean_matrix();
				std::vector<position_type> placed{};

				// ============================================================
				//  Step 1 — 放置起始房间
				// ============================================================

				element_of(matrix, {start_x, start_y}) = blueprint::RoomType::START;
				placed.emplace_back(start_x, start_y);

				// ============================================================
				//  Step 2 — 随机前沿扩展,生长主区域,保证所有房间与 START 连通
				// ============================================================

				while (placed.size() < main_region_count)
				{
					auto neighbors = get_unplaced_neighbors(matrix, placed);
					if (neighbors.empty())
					{
						break;
					}

					const auto chosen = manager::Random::int_inclusive(std::size_t{0}, neighbors.size() - 1);
					const auto position = neighbors[chosen];

					element_of(matrix, position) = blueprint::RoomType::STANDARD;
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
							const position_type boss_position{position_x, position_y};

							if (element_of(matrix, boss_position).has_value())
							{
								continue;
							}

							// BOSS 候选: 恰好与 1 个已放置房间相邻
							if (count_placed_neighbors(matrix, boss_position) != 1)
							{
								continue;
							}

							// 在 BOSS 候选四周寻找 EXIT 位置
							for (const auto [x, y]: Directions)
							{
								const position_type exit_position{boss_position.x + x, boss_position.y + y};

								if (not is_within_bounds(matrix, exit_position))
								{
									continue;
								}

								if (element_of(matrix, exit_position).has_value())
								{
									continue;
								}

								// EXIT 除了 BOSS 之外不能有任何已放置的邻居
								bool valid = true;
								for (const auto [next_x, next_y]: Directions)
								{
									const position_type exit_neighbor_position{exit_position.x + next_x, exit_position.y + next_y};

									// BOSS房间除外
									if (exit_neighbor_position == boss_position)
									{
										continue;
									}

									// 出界的邻居不算
									if (not is_within_bounds(matrix, exit_neighbor_position))
									{
										continue;
									}

									// 只要有一个已放置的邻居,就不合法
									if (not element_of(matrix, exit_neighbor_position).has_value())
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

					element_of(matrix, boss_position) = blueprint::RoomType::BOSS;
					element_of(matrix, exit_position) = blueprint::RoomType::EXIT;
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
						[&matrix](const position_type& position) noexcept -> bool
						{
							const auto& element = element_of(matrix, position);
							return element.has_value() and *element == blueprint::RoomType::STANDARD;
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

						element_of(matrix, key_position) = blueprint::RoomType::KEY;
						standards.pop_back();
					}

					// MERCHANT(可选,保留至少 1 个STANDARD)
					if (standards.size() >= 2)
					{
						const auto merchant_position = standards.back();

						element_of(matrix, merchant_position) = blueprint::RoomType::MERCHANT;
						standards.pop_back();
					}

					// BONUS(可选,保留至少 1 个STANDARD)
					if (standards.size() >= 2)
					{
						const auto bonus_position = standards.back();

						element_of(matrix, bonus_position) = blueprint::RoomType::BONUS;
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
				for (size_type y = 0; y < vertical_count; ++y)
				{
					std::string row{};

					row.reserve(horizontal_count);
					for (size_type x = 0; x < horizontal_count; ++x)
					{
						const position_type position{x, y};
						row.push_back(char_of(element_of(matrix, position)));
					}

					SPDLOG_INFO("{}", row);
				}
				SPDLOG_INFO("----- DUNGEON LAYOUT ----- ");

				return true;
			}

			// PROMETHEUS_PLATFORM_UNREACHABLE("Failed to generate dungeon layout after maximum attempts");
			SPDLOG_ERROR("Failed to generate dungeon layout after maximum attempts");
			return false;
		}
	}

	auto Level::generate(size_type horizontal_count, size_type vertical_count, size_type count, size_type start_x, size_type start_y) noexcept -> blueprint::Level
	{
		horizontal_count = std::ranges::clamp(horizontal_count, horizontal_min_count, horizontal_max_count);
		vertical_count = std::ranges::clamp(vertical_count, vertical_min_count, vertical_max_count);
		count = std::ranges::clamp(count, base_count, horizontal_count * vertical_count);
		start_x = std::ranges::min(start_x, horizontal_count - 1);
		start_y = std::ranges::min(start_y, vertical_count - 1);

		// 生成布局
		const auto matrix_memory_count = static_cast<std::size_t>(horizontal_count) * vertical_count;
		const auto matrix_memory = std::make_unique_for_overwrite<std::optional<blueprint::RoomType>[]>(matrix_memory_count);
		auto matrix = make_matrix(matrix_memory, horizontal_count, vertical_count);
		const auto succeed = generate_layout(
			horizontal_count,
			vertical_count,
			count,
			start_x,
			start_y,
			matrix,
			[matrix_memory_count , & matrix_memory] noexcept -> void
			{
				std::ranges::fill_n(matrix_memory.get(), static_cast<std::iter_difference_t<std::optional<blueprint::RoomType>*>>(matrix_memory_count), std::nullopt);
			}
		);
		PROMETHEUS_PLATFORM_ASSUME(succeed);

		// 生成房间
		blueprint::Level level{};
		level.rooms.reserve(count);
		for (size_type y = 0; y < vertical_count; ++y)
		{
			for (size_type x = 0; x < horizontal_count; ++x)
			{
				const position_type position{x, y};
				const auto& element = element_of(matrix, position);

				if (not element.has_value())
				{
					continue;
				}

				const auto type = *element;

				auto room = [&] noexcept -> blueprint::Room
				{
					switch (type)
					{
						case blueprint::RoomType::START:
						{
							// TODO
							return Room::standard(x, y);
						}
						case blueprint::RoomType::STANDARD:
						{
							return Room::standard(x, y);
						}
						case blueprint::RoomType::BOSS:
						{
							// TODO
							return Room::standard(x, y);
						}
						case blueprint::RoomType::KEY:
						{
							// TODO
							return Room::standard(x, y);
						}
						case blueprint::RoomType::MERCHANT:
						{
							// TODO
							return Room::standard(x, y);
						}
						case blueprint::RoomType::BONUS:
						{
							// TODO
							return Room::standard(x, y);
						}
						case blueprint::RoomType::EXIT:
						{
							// TODO
							return Room::standard(x, y);
						}
						default: // NOLINT(clang-diagnostic-covered-switch-default)
						{
							PROMETHEUS_PLATFORM_UNREACHABLE();
						}
					}
				}();

				level.rooms.emplace(type, std::move(room));
			}
		}

		return level;
	}
}
