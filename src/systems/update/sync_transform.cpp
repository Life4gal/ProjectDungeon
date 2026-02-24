// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/sync_transform.hpp>

#include <components/transform.hpp>

#include <systems/helper/physics_world.hpp>

#include <game/constants.hpp>
#include <game/user_data_entity.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::update
{
	auto sync_transform(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		using namespace components;
		using namespace systems;

		const auto world_id = helper::PhysicsWorld::id(registry);

		const auto body_events = b2World_GetBodyEvents(world_id);

		// move
		{
			const auto move_count = body_events.moveCount;
			for (int i = 0; i < move_count; ++i)
			{
				const auto& event = body_events.moveEvents[i];

				const auto entity = user_data_to_entity(event.userData);
				if (not registry.valid(entity))
				{
					continue;
				}

				const auto& [physics_position, physics_rotation] = event.transform;

				const auto position = Constant::from_physics(physics_position);
				const auto angle = sf::radians(b2Rot_GetAngle(physics_rotation));

				registry.replace<transform::Position>(entity, transform::Position{position});
				registry.replace<transform::Rotation>(entity, transform::Rotation{angle});
			}
		}
	}
}
