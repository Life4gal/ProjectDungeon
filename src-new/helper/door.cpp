// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/door.hpp>

#include <manager/event.hpp>

#include <event/door.hpp>

#include <component/room.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;
	
	auto Door::contact([[maybe_unused]] entt::registry& registry, const entt::entity door, const entt::entity other) noexcept -> void
	{
		SPDLOG_INFO(
			"门实体(0x{:08X})与实体(0x{:08X})接触!",
			entt::to_integral(door),
			entt::to_integral(other)
		);

		manager::Event::enqueue(event::door::Contacted{.door = door, .other = other});
	}

	auto Door::try_open(entt::registry& registry, const entt::entity door) noexcept -> void
	{
		// TODO: 如果门需要钥匙,检测玩家是否拥有钥匙?

		// physics
		if (const auto* physics_shape_door = registry.try_get<const door::PhysicsShapeDoor>(door))
		{
			auto filter = b2Shape_GetFilter(physics_shape_door->shape);
			filter.maskBits = blueprint::CollisionMask::door_open;

			b2Shape_SetFilter(physics_shape_door->shape, filter);
		}
		else
		{
			SPDLOG_WARN("门实体(0x{:08X})找不到物理体组件!这可能导致门无法通过!", entt::to_integral(door));
			return;
		}

		// TODO: animation

		manager::Event::enqueue(event::door::Opened{.door = door});
	}

	auto Door::try_close(entt::registry& registry, const entt::entity door) noexcept -> void
	{
		// physics
		if (const auto* physics_shape_door = registry.try_get<const door::PhysicsShapeDoor>(door))
		{
			auto filter = b2Shape_GetFilter(physics_shape_door->shape);
			filter.maskBits = blueprint::CollisionMask::door_close;

			b2Shape_SetFilter(physics_shape_door->shape, filter);
		}
		else
		{
			SPDLOG_WARN("门实体(0x{:08X})找不到物理体组件!这可能导致门无法阻止通过!", entt::to_integral(door));
			return;
		}

		// TODO: animation

		manager::Event::enqueue(event::door::Closed{.door = door});
	}
}
