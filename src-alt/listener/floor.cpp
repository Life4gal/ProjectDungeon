// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/floor.hpp>

#include <manager/event.hpp>
#include <manager/random.hpp>

#include <components/floor.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>

#if PROMETHEUS_COMPILER_DEBUG
#include <print>
#endif

namespace pd::listener
{
	namespace
	{
		using namespace components;

		using position_type = sf::Vector2u;
		using room_type = floor::RoomType;
		using infos_type = floor::RoomInfos;

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

		// 确定坐标是否在地图范围内
		[[nodiscard]] constexpr auto is_within_bounds(const position_type position) noexcept -> bool
		{
			return position.x > 0 and position.x < game::FloorHorizontalRoom and position.y > 0 and position.y < game::FloorVerticalRoom;
		}

		// 统计某个位置四周已放置(非NONE)房间的数量
		// 这也可以确定门的数量
		[[nodiscard]] constexpr auto count_placed_neighbors(const infos_type& infos, const position_type position) noexcept -> std::uint32_t
		{
			std::uint32_t count = 0;
			for (const auto [x, y]: Directions)
			{
				const auto next_position = position_type{position.x + x, position.y + y};

				if (not is_within_bounds(next_position))
				{
					continue;
				}

				if (infos[next_position.x, next_position.y].type == room_type::NONE)
				{
					continue;
				}

				count += 1;
			}

			return count;
		}

		// 获取某个位置四周已放置(非NONE)房间的坐标
		[[nodiscard]] constexpr auto get_placed_neighbors(const infos_type& infos, const position_type position) noexcept -> std::vector<position_type>
		{
			std::vector<position_type> neighbors{};
			for (const auto [x, y]: Directions)
			{
				const auto next_position = position_type{position.x + x, position.y + y};

				if (not is_within_bounds(next_position))
				{
					continue;
				}

				if (infos[next_position.x, next_position.y].type == room_type::NONE)
				{
					continue;
				}

				neighbors.push_back(next_position);
			}

			return neighbors;
		}

		// 获取某个位置四周未放置(NONE)房间的坐标
		[[nodiscard]] constexpr auto get_unplaced_neighbors(const infos_type& infos, const position_type position) noexcept -> std::vector<position_type>
		{
			std::vector<position_type> neighbors{};
			for (const auto [x, y]: Directions)
			{
				const auto next_position = position_type{position.x + x, position.y + y};

				if (not is_within_bounds(next_position))
				{
					continue;
				}

				if (infos[next_position.x, next_position.y].type != room_type::NONE)
				{
					continue;
				}

				neighbors.push_back(next_position);
			}

			return neighbors;
		}

		// 获取所有已放置位置四周未放置(NONE)房间的坐标
		[[nodiscard]] constexpr auto get_frontier(const infos_type& infos, const std::span<const position_type> placed) noexcept -> std::vector<position_type>
		{
			std::vector<position_type> frontier{};

			// 获取
			for (const auto& position: placed)
			{
				auto neighbors = get_unplaced_neighbors(infos, position);
				frontier.append_range(std::move(neighbors));
			}
			// 去重
			{
				std::ranges::sort(
					frontier,
					[](const position_type& a, const position_type& b) noexcept -> bool
					{
						return std::tie(a.x, a.y) < std::tie(b.x, b.y);
					}
				);

				const auto r = std::ranges::unique(frontier);
				frontier.erase(r.begin(), frontier.end());
			}

			return frontier;
		}

		// 生成所有房间布局
		[[nodiscard]] auto generate_floor(const position_type start_position, const std::uint32_t count) noexcept -> infos_type
		{
			PROMETHEUS_PLATFORM_ASSUME(is_within_bounds(start_position));
			PROMETHEUS_PLATFORM_ASSUME(count > game::FloorRoomBaseCount);
			PROMETHEUS_PLATFORM_ASSUME(count < game::FloorRoomMaxCount);

			// 主区域大小 = 总房间数 - BOSS - EXIT
			const auto main_region_count = count - 2;

			for (std::uint32_t attempt = 0; attempt < MaxRegenerationAttempts; ++attempt)
			{
#if PROMETHEUS_COMPILER_DEBUG
				std::println("Dungeon generation attempt {}/{}...", attempt + 1, MaxRegenerationAttempts);
#endif

				infos_type infos{};
				infos.fill({.type = room_type::NONE, .visited = false, .cleared = false});

				std::vector<position_type> placed{};

				// ============================================================
				//  Step 1 — 放置起始房间
				// ============================================================

				infos[start_position.x, start_position.y].type = room_type::START;
				placed.push_back(start_position);

				// ============================================================
				//  Step 2 — 随机前沿扩展,生长主区域,保证所有房间与 START 连通
				// ============================================================

				while (placed.size() < main_region_count)
				{
					auto frontier = get_frontier(infos, placed);
					if (frontier.empty())
					{
						break;
					}

					const auto chosen = manager::Random::int_inclusive(std::size_t{0}, frontier.size() - 1);
					const auto position = frontier[chosen];

					infos[position.x, position.y].type = room_type::STANDARD;
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
						std::uint32_t manhattan_distance;
					};

					std::vector<Candidate> candidates{};

					for (std::uint32_t position_y = 0; position_y < game::FloorVerticalRoom; ++position_y)
					{
						for (std::uint32_t position_x = 0; position_x < game::FloorHorizontalRoom; ++position_x)
						{
							const auto boss_position = position_type{position_x, position_y};

							if (infos[boss_position.x, boss_position.y].type != room_type::NONE)
							{
								continue;
							}

							// BOSS 候选: 恰好与 1 个已放置房间相邻
							if (count_placed_neighbors(infos, boss_position) != 1)
							{
								continue;
							}

							// 在 BOSS 候选四周寻找 EXIT 位置
							for (const auto [x, y]: Directions)
							{
								const auto exit_position = position_type{boss_position.x + x, boss_position.y + y};

								if (not is_within_bounds(exit_position))
								{
									continue;
								}

								if (infos[exit_position.x, exit_position.y].type != room_type::NONE)
								{
									continue;
								}

								// EXIT 除了 BOSS 之外不能有任何已放置的邻居
								bool valid = true;
								for (const auto [next_x, next_y]: Directions)
								{
									const auto exit_neighbor_position = position_type{exit_position.x + next_x, exit_position.y + next_y};

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
									if (infos[exit_neighbor_position.x, exit_neighbor_position.y].type == room_type::NONE)
									{
										continue;
									}

									valid = false;
									break;
								}

								if (valid)
								{
									const auto diff_x = static_cast<int>(boss_position.x) - static_cast<int>(start_position.x);
									const auto diff_y = static_cast<int>(boss_position.y) - static_cast<int>(start_position.y);

									const auto distance = std::abs(diff_x) + std::abs(diff_y);
									candidates.emplace_back(boss_position, exit_position, static_cast<std::uint32_t>(distance));
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

					infos[boss_position.x, boss_position.y].type = room_type::BOSS;
					infos[exit_position.x, exit_position.y].type = room_type::EXIT;
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
						[&infos](const auto& position) noexcept -> bool
						{
							return infos[position.x, position.y].type == room_type::STANDARD;
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

						infos[key_position.x, key_position.y].type = room_type::KEY;
						standards.pop_back();
					}

					// MERCHANT(可选,保留至少 1 个STANDARD)
					if (standards.size() >= 2)
					{
						const auto merchant_position = standards.back();

						infos[merchant_position.x, merchant_position.y].type = room_type::MERCHANT;
						standards.pop_back();
					}

					// BONUS(可选,保留至少 1 个STANDARD)
					if (standards.size() >= 2)
					{
						const auto bonus_position = standards.back();

						infos[bonus_position.x, bonus_position.y].type = room_type::BONUS;
						standards.pop_back();
					}
				}

#if PROMETHEUS_COMPILER_DEBUG
				const auto char_of = [](const room_type type) noexcept -> char
				{
					switch (type)
					{
						case room_type::NONE:
						{
							return '.';
						}
						case room_type::START:
						{
							return 'S';
						}
						case room_type::STANDARD:
						{
							return '#';
						}
						case room_type::BOSS:
						{
							return 'B';
						}
						case room_type::KEY:
						{
							return 'K';
						}
						case room_type::MERCHANT:
						{
							return 'M';
						}
						case room_type::BONUS:
						{
							return 'R';
						}
						case room_type::EXIT:
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
					char_of(room_type::NONE),
					char_of(room_type::START),
					char_of(room_type::STANDARD),
					char_of(room_type::BOSS),
					char_of(room_type::KEY),
					char_of(room_type::MERCHANT),
					char_of(room_type::BONUS),
					char_of(room_type::EXIT)
				);

				for (std::uint32_t y = 0; y < game::FloorVerticalRoom; ++y)
				{
					for (std::uint32_t x = 0; x < game::FloorVerticalRoom; ++x)
					{
						std::print("{} ", char_of(infos[x, y].type));
					}

					std::println();
				}

				std::println();
#endif

				return infos;
			}

			PROMETHEUS_PLATFORM_UNREACHABLE("Failed to generate dungeon layout after maximum attempts");
		}
	}

	auto Floor::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<floor::GenerateRequest, &Floor::on_generate_request>(registry);
		Event::subscribe<floor::DestroyRequest, &Floor::on_destroy_request>(registry);
		Event::subscribe<floor::Entered, &Floor::on_enter_room>(registry);
		Event::subscribe<floor::Cleared, &Floor::on_clear_room>(registry);
		Event::subscribe<floor::Left, &Floor::on_leave_room>(registry);
	}

	auto Floor::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<floor::GenerateRequest, &Floor::on_generate_request>(registry);
		Event::unsubscribe<floor::DestroyRequest, &Floor::on_destroy_request>(registry);
		Event::unsubscribe<floor::Entered, &Floor::on_enter_room>(registry);
		Event::unsubscribe<floor::Cleared, &Floor::on_clear_room>(registry);
		Event::unsubscribe<floor::Left, &Floor::on_leave_room>(registry);
	}

	auto Floor::on_generate_request(entt::registry& registry, const events::floor::GenerateRequest& event) noexcept -> void
	{
		const auto infos = generate_floor({event.start_x, event.start_y}, event.count);

		registry.ctx().insert_or_assign<floor::RoomInfos>(floor::RoomInfos{infos});
		registry.ctx().insert_or_assign<floor::RoomEntities>({});
		registry.ctx().insert_or_assign<floor::RoomCount>(floor::RoomCount{.count = event.count});
		registry.ctx().insert_or_assign<floor::CurrentRoom>(floor::CurrentRoom{.x = event.start_x, .y = event.start_y});
	}

	auto Floor::on_destroy_request(entt::registry& registry, [[maybe_unused]] const events::floor::DestroyRequest& event) noexcept -> void
	{
		registry.ctx().erase<floor::CurrentRoom>();
		registry.ctx().erase<floor::RoomCount>();
		registry.ctx().erase<floor::RoomEntities>();
		registry.ctx().erase<floor::RoomInfos>();
	}

	auto Floor::on_enter_room(entt::registry& registry, const events::floor::Entered& event) noexcept -> void
	{
		using namespace manager;
		// using namespace events;

		const auto& infos = registry.ctx().get<const floor::RoomInfos>();
		const auto& entities = registry.ctx().get<const floor::RoomEntities>();

		const auto& info = infos[event.x, event.y];
		const auto& entity = entities[event.x, event.y];

		Event::enqueue(events::room::Active{.info = std::cref(info), .entity = std::cref(entity)});
	}

	auto Floor::on_clear_room(entt::registry& registry, const events::floor::Cleared& event) noexcept -> void
	{
		using namespace manager;
		// using namespace events;

		const auto& infos = registry.ctx().get<const floor::RoomInfos>();
		const auto& entities = registry.ctx().get<const floor::RoomEntities>();

		const auto& info = infos[event.x, event.y];
		const auto& entity = entities[event.x, event.y];

		Event::enqueue(events::room::Cleared{.info = std::cref(info), .entity = std::cref(entity)});
	}

	auto Floor::on_leave_room(entt::registry& registry, const events::floor::Left& event) noexcept -> void
	{
		using namespace manager;
		// using namespace events;

		const auto& infos = registry.ctx().get<const floor::RoomInfos>();
		const auto& entities = registry.ctx().get<const floor::RoomEntities>();

		const auto& info = infos[event.x, event.y];
		const auto& entity = entities[event.x, event.y];

		Event::enqueue(events::room::Inactive{.info = std::cref(info), .entity = std::cref(entity)});
	}
}
