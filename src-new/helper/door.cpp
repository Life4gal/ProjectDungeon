// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/door.hpp>

#include <manager/event.hpp>

#include <event/door.hpp>

#include <component/door.hpp>

#include <helper/room.hpp>

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

	auto Door::sense(entt::registry& registry, const entt::entity door, const entt::entity other) noexcept -> void
	{
		// 无视非玩家实体
		// 理论上我们设置好了掩码,这里一定是玩家实体
		if (not registry.all_of<tags::Player>(other))
		{
			return;
		}

		manager::Event::enqueue(event::door::Sensed{.door = door, .player = other});

		const auto [room] = registry.get<const door::Room>(door);
		const auto [target_room] = registry.get<const door::TargetRoom>(door);

		Room::leave(registry, room, door);
		Room::enter(registry, target_room, door);
	}

	auto Door::try_open(entt::registry& registry, const entt::entity door) noexcept -> void
	{
		// TODO: 如果门需要钥匙,检测玩家是否拥有钥匙?

		// collision
		if (const auto* door_shape_id = registry.try_get<const door::DoorShapeId>(door))
		{
			auto filter = b2Shape_GetFilter(door_shape_id->shape_id);
			filter.maskBits = std::to_underlying(blueprint::CollisionMask::DOOR_OPENED);

			b2Shape_SetFilter(door_shape_id->shape_id, filter);

			SPDLOG_INFO("门(0x{:08X})已开启", entt::to_integral(door));
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
		// collision
		if (const auto* door_shape_id = registry.try_get<const door::DoorShapeId>(door))
		{
			auto filter = b2Shape_GetFilter(door_shape_id->shape_id);
			filter.maskBits = std::to_underlying(blueprint::CollisionMask::DOOR);

			b2Shape_SetFilter(door_shape_id->shape_id, filter);

			SPDLOG_INFO("门(0x{:08X})已关闭", entt::to_integral(door));
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
