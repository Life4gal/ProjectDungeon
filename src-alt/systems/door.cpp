// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/door.hpp>

#include <game/constants.hpp>

#include <components/door.hpp>
#include <components/physics_world.hpp>
#include <components/transform.hpp>

#include <systems/physics_world.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems
{
	auto Door::open(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		for (const auto view = registry.view<door::State, physics_world::BodyId>(
			     entt::exclude<
				     // 排除锁上的门
				     door::Locked
			     >
		     );
		     const auto [entity, state, body_id]: view.each())
		{
			if (state == door::State::CLOSING or state == door::State::CLOSED)
			{
				state = door::State::OPENING;
				registry.emplace_or_replace<door::Timer>(entity, sf::Time::Zero);
			}

			// 移除门的物理碰撞体
			PhysicsWorld::deattach(registry, entity);
		}
	}

	auto Door::close(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		for (const auto view = registry.view<door::State, transform::Position, transform::Scale, transform::Rotation>();
		     const auto [entity, state, position, scale, rotation]: view.each())
		{
			// 理论上其不应该有物理碰撞体,如果有则说明之前的门没有被正确移除
			PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity));

			if (state == door::State::OPENING or state == door::State::OPENED)
			{
				state = door::State::CLOSING;
				registry.emplace_or_replace<door::Timer>(entity, sf::Time::Zero);
			}

			// 创建门的物理碰撞体
			const auto world_id = PhysicsWorld::id(registry);

			auto body_def = b2DefaultBodyDef();
			body_def.type = b2_staticBody;
			body_def.position = PhysicsWorld::physics_position_of(position.position);
			body_def.rotation = PhysicsWorld::physics_rotation_of(rotation.rotation);
			body_def.userData = PhysicsWorld::to_user_data(entity);

			PhysicsWorld::attach(registry, entity, world_id, body_def);

			const auto [half_width, half_height] = PhysicsWorld::physics_size_of({game::RoomTileWidth * 0.5f * scale.scale.x, game::RoomTileHeight * 0.5f * scale.scale.y});
			const auto box = b2MakeBox(half_width, half_height);

			auto shape_def = b2DefaultShapeDef();
			shape_def.density = 0;
			shape_def.material.friction = 0;

			PhysicsWorld::attach_shape(registry, entity, shape_def, box);
		}
	}
}
