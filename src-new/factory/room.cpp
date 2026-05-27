// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/room.hpp>

#include <component/room.hpp>
#include <component/enemy.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/collision.hpp>
#include <factory/door_sensor.hpp>
#include <factory/tile.hpp>
#include <factory/enemy.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	namespace
	{
		constexpr blueprint::CollisionBodyDef BodyDef
		{
				.type = blueprint::CollisionBodyType::STATIC,
				.fixed_rotation = true,
				.is_bullet = false,
		};
		constexpr blueprint::CollisionShapeDef BoundingShapeDef
		{
				.material = {.friction = 0.3f, .restitution = 0},
				.density = 0,
				.category = blueprint::CollisionCategory::WALL,
				.mask = blueprint::CollisionMask::WALL,
				.is_sensor = true,
				.enable_sensor_events = false,
				.enable_contact_events = false,
		};
	}

	auto Room::spawn(entt::registry& registry, const blueprint::Room& room) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// ============================================
		// 位置 + 大小
		// ============================================

		// layout position
		registry.emplace<room::LayoutPosition>(entity, sf::Vector2u{room.layout_position.x, room.layout_position.y});
		// position
		registry.emplace<room::Position>(entity, sf::Vector2f{room.position.x, room.position.y});
		// size
		registry.emplace<room::Size>(entity, sf::Vector2f{room.size.width, room.size.height});

		// ============================================
		// 门感应区
		// ============================================

		{
			const auto neighbors_value = std::to_underlying(room.neighbors);
			// const auto neighbors_count = std::popcount(neighbors_value);

			auto& [sensors] = registry.emplace<room::DoorSensors>(entity);
			sensors.fill(entt::null);

			const auto do_create = [&](const blueprint::DirectionMask mask, const blueprint::Direction direction) noexcept -> void
			{
				if (neighbors_value & std::to_underlying(mask))
				{
					const auto& def = room.door_sensors[std::to_underlying(direction)];
					auto& sensor = sensors[std::to_underlying(direction)];

					sensor = DoorSensor::spawn(registry, def, direction);

					// 设置门感应器所属房间实体
					registry.emplace<door_sensor::Room>(sensor, entity); // NOLINT(readability-suspicious-call-argument)
					// 先将门感应器的目标房间设置为当前房间
					// 如此在factory::Level我们便可以确定遍历到的门感应器实体属于哪个房间
					registry.emplace<door_sensor::TargetRoom>(sensor, entity); // NOLINT(readability-suspicious-call-argument)
				}
			};

			do_create(blueprint::DirectionMask::NORTH, blueprint::Direction::NORTH);
			do_create(blueprint::DirectionMask::SOUTH, blueprint::Direction::SOUTH);
			do_create(blueprint::DirectionMask::WEST, blueprint::Direction::WEST);
			do_create(blueprint::DirectionMask::EAST, blueprint::Direction::EAST);
		}

		// ============================================
		// collision & 房间边界
		// ============================================

		{
			const auto body_id = detail::create_attach(registry, entity, BodyDef, room.position);

			auto& [bounding] = registry.emplace<room::Bounding>(entity);
			bounding.reserve(room.bounding.size());

			for (const auto& [shape]: room.bounding)
			{
				const auto shape_id = detail::create(body_id, BoundingShapeDef, shape);

				bounding.emplace_back(shape_id);
			}
		}

		// ============================================
		// 瓦片
		// ============================================

		{
			auto& [tiles] = registry.emplace<room::Tiles>(entity);
			tiles.reserve(room.tiles.size());

			for (const auto& tile: room.tiles)
			{
				const auto e = Tile::spawn(registry, tile);

				tiles.emplace_back(e);
			}
		}

		// ============================================
		// 敌人
		// ============================================

		{
			auto& [enemies] = registry.emplace<room::Enemies>(entity);
			enemies.reserve(room.enemies.size());

			for (const auto& enemy: room.enemies)
			{
				const auto enemy_entity = Enemy::spawn(registry, enemy);

				// 设置敌人所属房间
				registry.emplace<enemy::Room>(enemy_entity, entity); // NOLINT(readability-suspicious-call-argument)

				enemies.push_back(enemy_entity);
			}
		}

		// tags
		registry.emplace<tags::Room>(entity);

		return entity;
	}

	auto Room::destroy_all(entt::registry& registry) noexcept -> void
	{
		// 门感应区
		DoorSensor::destroy_all(registry);
		// 瓦片
		Tile::destroy_all(registry);
		// 敌人
		Enemy::destroy_all(registry);

		const auto view = registry.view<tags::Room>();
		registry.destroy(view.begin(), view.end());
	}
}
