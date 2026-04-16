// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <blueprint/floor.hpp>

#include <algorithm>
#include <ranges>

#include <manager/random.hpp>

#include <prometheus/platform/os.hpp>
#include <SFML/System/Vector2.hpp>

#if PROMETHEUS_COMPILER_DEBUG
#include <print>
#endif

namespace pd::blueprint
{
	namespace
	{
		// 如果无法满足BOSS房间+出口房间组合,重新生成前的最大尝试次数
		constexpr std::uint32_t MaxRegenerationAttempts = 10;

		// 各个方向的坐标偏移
		constexpr std::array<sf::Vector2i, 4> Directions
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
	}

	auto Floor::is_within_bounds(const position_type position) noexcept -> bool
	{
		return position.x > 0 and position.x < game::FloorHorizontalRoom and position.y > 0 and position.y < game::FloorVerticalRoom;
	}

	auto Floor::generate(position_type start_position, size_type count) noexcept -> layout_type
	{
		// PROMETHEUS_PLATFORM_ASSUME(is_within_bounds(start_position));
		// PROMETHEUS_PLATFORM_ASSUME(count > game::FloorRoomBaseCount);
		// PROMETHEUS_PLATFORM_ASSUME(count < game::FloorRoomMaxCount);
		if (not is_within_bounds(start_position))
		{
			start_position = {.x = 0, .y = 0};
		}
		count = std::ranges::clamp(count, size_type{game::FloorRoomBaseCount}, size_type{game::FloorRoomMaxCount});

		// 主区域大小 = 总房间数 - BOSS - EXIT
		const auto main_region_count = count - 2;

		for (std::uint32_t attempt = 0; attempt < MaxRegenerationAttempts; ++attempt)
		{
#if PROMETHEUS_COMPILER_DEBUG
			std::println("Dungeon generation attempt {}/{}...", attempt + 1, MaxRegenerationAttempts);
#endif

			layout_type layout{};
			layout.fill(RoomType::NONE);

			std::vector<position_type> placed{};

			// ============================================================
			//  Step 1 — 放置起始房间
			// ============================================================

			layout[start_position.x, start_position.y] = RoomType::START;
			placed.push_back(start_position);

			// ============================================================
			//  Step 2 — 随机前沿扩展,生长主区域,保证所有房间与 START 连通
			// ============================================================

			while (placed.size() < main_region_count)
			{
				auto frontier = get_unplaced_neighbors(layout, placed);
				if (frontier.empty())
				{
					break;
				}

				const auto chosen = manager::Random::int_inclusive(std::size_t{0}, frontier.size() - 1);
				const auto position = frontier[chosen];

				layout[position.x, position.y] = RoomType::STANDARD;
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

				for (std::uint32_t position_y = 0; position_y < game::FloorVerticalRoom; ++position_y)
				{
					for (std::uint32_t position_x = 0; position_x < game::FloorHorizontalRoom; ++position_x)
					{
						const position_type boss_position{.x = position_x, .y = position_y};

						if (layout[boss_position.x, boss_position.y] != RoomType::NONE)
						{
							continue;
						}

						// BOSS 候选: 恰好与 1 个已放置房间相邻
						if (count_placed_neighbors(layout, boss_position) != 1)
						{
							continue;
						}

						// 在 BOSS 候选四周寻找 EXIT 位置
						for (const auto [x, y]: Directions)
						{
							const position_type exit_position{.x = boss_position.x + x, .y = boss_position.y + y};

							if (not is_within_bounds(exit_position))
							{
								continue;
							}

							if (layout[exit_position.x, exit_position.y] != RoomType::NONE)
							{
								continue;
							}

							// EXIT 除了 BOSS 之外不能有任何已放置的邻居
							bool valid = true;
							for (const auto [next_x, next_y]: Directions)
							{
								const position_type exit_neighbor_position{.x = exit_position.x + next_x, .y = exit_position.y + next_y};

								// BOSS房间除外
								if (exit_neighbor_position == boss_position)
								{
									continue;
								}

								// 出界的邻居不算
								if (not is_within_bounds(exit_neighbor_position))
								{
									continue;
								}

								// 只要有一个已放置的邻居,就不合法
								if (layout[exit_neighbor_position.x, exit_neighbor_position.y] == RoomType::NONE)
								{
									continue;
								}

								valid = false;
								break;
							}

							if (valid)
							{
								const auto diff_x = static_cast<std::make_signed_t<size_type>>(boss_position.x) - static_cast<std::make_signed_t<size_type>>(start_position.x);
								const auto diff_y = static_cast<std::make_signed_t<size_type>>(boss_position.y) - static_cast<std::make_signed_t<size_type>>(start_position.y);

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

				layout[boss_position.x, boss_position.y] = RoomType::BOSS;
				layout[exit_position.x, exit_position.y] = RoomType::EXIT;
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
						return layout[position.x, position.y] == RoomType::STANDARD;
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

					layout[key_position.x, key_position.y] = RoomType::KEY;
					standards.pop_back();
				}

				// MERCHANT(可选,保留至少 1 个STANDARD)
				if (standards.size() >= 2)
				{
					const auto merchant_position = standards.back();

					layout[merchant_position.x, merchant_position.y] = RoomType::MERCHANT;
					standards.pop_back();
				}

				// BONUS(可选,保留至少 1 个STANDARD)
				if (standards.size() >= 2)
				{
					const auto bonus_position = standards.back();

					layout[bonus_position.x, bonus_position.y] = RoomType::BONUS;
					standards.pop_back();
				}
			}

#if PROMETHEUS_COMPILER_DEBUG
			const auto char_of = [](const RoomType type) noexcept -> char
			{
				switch (type)
				{
					case RoomType::NONE:
					{
						return '.';
					}
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

			std::println("----- DUNGEON LAYOUT ----- ");
			std::println(
				"-NONE={} -START={} -STANDARD={} -BOSS={} -KEY={} -MERCHANT={} -BONUS={} -EXIT={}",
				char_of(RoomType::NONE),
				char_of(RoomType::START),
				char_of(RoomType::STANDARD),
				char_of(RoomType::BOSS),
				char_of(RoomType::KEY),
				char_of(RoomType::MERCHANT),
				char_of(RoomType::BONUS),
				char_of(RoomType::EXIT)
			);

			for (std::uint32_t y = 0; y < game::FloorVerticalRoom; ++y)
			{
				for (std::uint32_t x = 0; x < game::FloorVerticalRoom; ++x)
				{
					std::print("{} ", char_of(layout[x, y]));
				}

				std::println();
			}

			std::println();
#endif

			return layout;
		}

		PROMETHEUS_PLATFORM_UNREACHABLE("Failed to generate dungeon layout after maximum attempts");
	}

	auto Floor::count_placed_neighbors(const layout_type& layout, const position_type position) noexcept -> size_type
	{
		size_type count = 0;
		for (const auto [x, y]: Directions)
		{
			const position_type next_position{.x = position.x + x, .y = position.y + y};

			if (not is_within_bounds(next_position))
			{
				continue;
			}

			if (layout[next_position.x, next_position.y] == RoomType::NONE)
			{
				continue;
			}

			count += 1;
		}

		return count;
	}

	auto Floor::get_placed_neighbors(const layout_type& layout, const position_type position) noexcept -> std::vector<position_type>
	{
		std::vector<position_type> neighbors{};
		for (const auto [x, y]: Directions)
		{
			const position_type next_position{.x = position.x + x, .y = position.y + y};

			if (not is_within_bounds(next_position))
			{
				continue;
			}

			if (layout[next_position.x, next_position.y] == RoomType::NONE)
			{
				continue;
			}

			neighbors.push_back(next_position);
		}

		return neighbors;
	}

	auto Floor::get_unplaced_neighbors(const layout_type& layout, const position_type position) noexcept -> std::vector<position_type>
	{
		std::vector<position_type> neighbors{};
		for (const auto [x, y]: Directions)
		{
			const position_type next_position{.x = position.x + x, .y = position.y + y};

			if (not is_within_bounds(next_position))
			{
				continue;
			}

			if (layout[next_position.x, next_position.y] != RoomType::NONE)
			{
				continue;
			}

			neighbors.push_back(next_position);
		}

		return neighbors;
	}

	auto Floor::get_unplaced_neighbors(const layout_type& layout, const std::span<const position_type> positions) noexcept -> std::vector<position_type>
	{
		std::vector<position_type> all_neighbors{};

		// 获取
		for (const auto& position: positions)
		{
			auto neighbors = get_unplaced_neighbors(layout, position);
			all_neighbors.append_range(std::move(neighbors));
		}
		// 去重
		{
			std::ranges::sort(all_neighbors);

			const auto r = std::ranges::unique(all_neighbors);
			all_neighbors.erase(r.begin(), all_neighbors.end());
		}

		return all_neighbors;
	}
}
