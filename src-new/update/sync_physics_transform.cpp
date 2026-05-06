// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/sync_physics_transform.hpp>

#include <utility/physics.hpp>

#include <component/physics_body.hpp>
#include <component/position.hpp>
#include <component/sprite.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::update
{
	using namespace component;

	auto sync_physics_transform(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		using utility::Physics;

		const auto world_id = Physics::world_id;

		const auto body_events = b2World_GetBodyEvents(world_id);
		const auto move_count = body_events.moveCount;

		// move
		for (int i = 0; i < move_count; ++i)
		{
			const auto& event = body_events.moveEvents[i];
			const auto entity = Physics::to_entity(event.userData);

			if (not registry.valid(entity))
			{
				continue;
			}

			const auto& [position, rotation] = event.transform;
			const auto pixels_position = Physics::from_physics(position);
			const auto pixels_rotation = Physics::from_physics(rotation);

			// 世界坐标
			registry.emplace_or_replace<position::World>(entity, pixels_position);
			// 屏幕坐标
			//
			// 渲染的旋转
			registry.emplace_or_replace<sprite::RenderRotation>(entity, pixels_rotation);
		}
	}
}
