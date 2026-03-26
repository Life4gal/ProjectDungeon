// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/room.hpp>

#include <algorithm>
#include <span>
#include <vector>

#include <manager/random.hpp>

#include <prometheus/platform/os.hpp>

#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>

#if PROMETHEUS_COMPILER_DEBUG
#include <print>
#endif

namespace pd::systems
{
	namespace
	{
		class RoomGenerator final
		{
		public:
			using room_type = components::room::RoomType;
			using room_infos_type = components::room::RoomInfos::matrix_type;

			using position_type = Room::room_position_type;

			// 如果无法满足BOSS房间+出口房间组合,重新生成前的最大尝试次数
			constexpr static std::uint32_t max_regeneration_attempts = 10;

			constexpr static std::array<sf::Vector2i, 4> directions
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

		private:
#if PROMETHEUS_COMPILER_DEBUG
			[[nodiscard]] constexpr static auto char_of(const room_type type) noexcept -> char
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
			}
#endif

			// 坐标是否在地图范围内
			[[nodiscard]] constexpr static auto is_within_bounds(const position_type position) noexcept -> bool
			{
				return position.x > 0 and position.x < game::DungeonFloorWidth and position.y > 0 and position.y < game::DungeonFloorHeight;
			}

			// 统计某个位置四周已放置(非NONE)房间的数量
			[[nodiscard]] constexpr static auto count_placed_neighbors(const room_infos_type& infos, const position_type position) noexcept -> std::uint32_t
			{
				std::uint32_t count = 0;
				for (const auto [x, y]: directions)
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

			// 收集所有与已放置房间相邻的空位
			[[nodiscard]] constexpr static auto build_frontier(const room_infos_type& infos, const std::span<const position_type> placed) noexcept -> std::vector<position_type>
			{
				std::vector<position_type> frontier{};
				for (const auto& position: placed)
				{
					for (const auto [x, y]: directions)
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

						frontier.push_back(next_position);
					}
				}

				return frontier;
			}

		public:
			[[nodiscard]] static auto generate(const position_type start_position, const std::uint32_t count) noexcept -> room_infos_type
			{
				PROMETHEUS_PLATFORM_ASSUME(is_within_bounds(start_position));
				PROMETHEUS_PLATFORM_ASSUME(count > game::DungeonRoomBaseCount);
				PROMETHEUS_PLATFORM_ASSUME(count < game::DungeonRoomMaxCount);

				// 主区域大小 = 总房间数 - BOSS - EXIT
				const auto main_region_count = count - 2;

				for (std::uint32_t attempt = 0; attempt < max_regeneration_attempts; ++attempt)
				{
#if PROMETHEUS_COMPILER_DEBUG
					std::println("Dungeon generation attempt {}/{}...", attempt + 1, max_regeneration_attempts);
#endif

					room_infos_type infos{};
					infos.fill({.type = room_type::NONE, .visited = false, .cleared = false, .boss_door_opened = false});

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
						auto frontier = build_frontier(infos, placed);
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

						for (std::uint32_t position_y = 0; position_y < game::DungeonFloorHeight; ++position_y)
						{
							for (std::uint32_t position_x = 0; position_x < game::DungeonFloorWidth; ++position_x)
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
								for (const auto [x, y]: directions)
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
									for (const auto [next_x, next_y]: directions)
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

						std::ranges::shuffle(standards, manager::Random::instance());

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

					for (std::uint32_t y = 0; y < game::DungeonFloorHeight; ++y)
					{
						for (std::uint32_t x = 0; x < game::DungeonFloorWidth; ++x)
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
		};
	}

	auto Room::create(entt::registry& registry, const room_position_type start_position, const std::uint32_t count) noexcept -> void
	{
		using namespace components;

		// 所有房间信息
		registry.ctx().emplace<room::RoomInfos>(RoomGenerator::generate(start_position, count));

		// 所有房间缓存
		registry.ctx().emplace<room::RoomCacheTiles>();
		registry.ctx().emplace<room::RoomCacheDoors>();
		registry.ctx().emplace<room::RoomCacheChests>();
		registry.ctx().emplace<room::RoomCacheDestroyableObjects>();
		registry.ctx().emplace<room::RoomCacheItems>();
		registry.ctx().emplace<room::RoomCacheCorpses>();
		registry.ctx().emplace<room::RoomCacheBloodStains>();

		// 当前所在房间坐标
		registry.ctx().emplace<room::RoomCurrentPosition>(start_position.x, start_position.y);
	}

	auto Room::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		// 当前所在房间坐标
		registry.ctx().erase<room::RoomCurrentPosition>();

		// 所有房间缓存
		registry.ctx().erase<room::RoomCacheBloodStains>();
		registry.ctx().erase<room::RoomCacheCorpses>();
		registry.ctx().erase<room::RoomCacheItems>();
		registry.ctx().erase<room::RoomCacheDestroyableObjects>();
		registry.ctx().erase<room::RoomCacheChests>();
		registry.ctx().erase<room::RoomCacheDoors>();
		registry.ctx().erase<room::RoomCacheTiles>();

		// 所有房间信息
		registry.ctx().erase<room::RoomInfos>();
	}

	auto Room::on_contact_chest(entt::registry& registry, const events::ChestContacted& event) noexcept -> void
	{
		using namespace components;

		// 获取缓存
		auto& [caches] = get_cache_chests(registry);

		PROMETHEUS_PLATFORM_ASSUME(event.chest_index < caches.size());

		auto& cache = caches[event.chest_index];
		cache.opened = true;
	}

	auto Room::get_info(entt::registry& registry, const room_position_type position) noexcept -> room_info&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [infos] = registry.ctx().get<room::RoomInfos>();

		return infos[position.x, position.y];
	}

	auto Room::get_info(entt::registry& registry) noexcept -> room_info&
	{
		using namespace components;

		return get_info(registry, get_position(registry));
	}

	auto Room::get_cache_tiles(entt::registry& registry, const room_position_type position) noexcept -> cache_tiles&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [caches] = registry.ctx().get<room::RoomCacheTiles>();

		return caches[position.x, position.y];
	}

	auto Room::get_cache_tiles(entt::registry& registry) noexcept -> cache_tiles&
	{
		using namespace components;

		return get_cache_tiles(registry, get_position(registry));
	}

	auto Room::get_cache_doors(entt::registry& registry, const room_position_type position) noexcept -> cache_doors&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [caches] = registry.ctx().get<room::RoomCacheDoors>();

		return caches[position.x, position.y];
	}

	auto Room::get_cache_doors(entt::registry& registry) noexcept -> cache_doors&
	{
		using namespace components;

		return get_cache_doors(registry, get_position(registry));
	}

	auto Room::get_cache_chests(entt::registry& registry, const room_position_type position) noexcept -> cache_chests&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [caches] = registry.ctx().get<room::RoomCacheChests>();

		return caches[position.x, position.y];
	}

	auto Room::get_cache_chests(entt::registry& registry) noexcept -> cache_chests&
	{
		using namespace components;

		return get_cache_chests(registry, get_position(registry));
	}

	auto Room::get_cache_destroyable_objects(entt::registry& registry, const room_position_type position) noexcept -> cache_destroyable_objects&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [caches] = registry.ctx().get<room::RoomCacheDestroyableObjects>();

		return caches[position.x, position.y];
	}

	auto Room::get_cache_destroyable_objects(entt::registry& registry) noexcept -> cache_destroyable_objects&
	{
		using namespace components;

		return get_cache_destroyable_objects(registry, get_position(registry));
	}

	auto Room::get_cache_items(entt::registry& registry, const room_position_type position) noexcept -> cache_items&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [caches] = registry.ctx().get<room::RoomCacheItems>();

		return caches[position.x, position.y];
	}

	auto Room::get_cache_items(entt::registry& registry) noexcept -> cache_items&
	{
		using namespace components;

		return get_cache_items(registry, get_position(registry));
	}

	auto Room::get_cache_corpses(entt::registry& registry, const room_position_type position) noexcept -> cache_corpses&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [caches] = registry.ctx().get<room::RoomCacheCorpses>();

		return caches[position.x, position.y];
	}

	auto Room::get_cache_corpses(entt::registry& registry) noexcept -> cache_corpses&
	{
		using namespace components;

		return get_cache_corpses(registry, get_position(registry));
	}

	auto Room::get_cache_blood_stains(entt::registry& registry, const room_position_type position) noexcept -> cache_blood_stains&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(position.x < game::DungeonFloorWidth and position.y < game::DungeonFloorHeight);

		auto& [caches] = registry.ctx().get<room::RoomCacheBloodStains>();

		return caches[position.x, position.y];
	}

	auto Room::get_cache_blood_stains(entt::registry& registry) noexcept -> cache_blood_stains&
	{
		using namespace components;

		return get_cache_blood_stains(registry, get_position(registry));
	}

	auto Room::get_position(entt::registry& registry) noexcept -> sf::Vector2u
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<room::RoomCurrentPosition>());

		const auto [current_position_x, current_position_y] = registry.ctx().get<room::RoomCurrentPosition>();

		return {current_position_x, current_position_y};
	}

	auto Room::is_walkable(const collision_logical collision_logical) noexcept -> bool
	{
		return collision_logical == collision_logical::FLOOR;
	}

	auto Room::is_flyable(const collision_logical collision_logical) noexcept -> bool
	{
		return collision_logical != collision_logical::WALL;
	}

	auto Room::is_passable(const collision_logical collision_logical) noexcept -> bool
	{
		return collision_logical != collision_logical::WALL and collision_logical != collision_logical::OBSTACLE;
	}
}
