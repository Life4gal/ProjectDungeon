// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/door.hpp>

#include <manager/event.hpp>

#include <event/door.hpp>

#include <component/door.hpp>

#include <accessor/dynamic_sprite.hpp>

#include <helper/room.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	auto Door::contact([[maybe_unused]] entt::registry& registry, const entt::entity door, const entt::entity other) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Door>(door));

		SPDLOG_INFO(
			"门实体(0x{:08X})与实体(0x{:08X})接触!",
			entt::to_integral(door),
			entt::to_integral(other)
		);

		manager::Event::enqueue(event::door::Contacted{.door = door, .other = other});

		// if (player->has_key)
		// {
		// 	try_open(registry, door);
		// }
	}

	auto Door::sense(entt::registry& registry, const entt::entity door, const entt::entity other) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Door>(door));

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
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Door>(door));

		const auto state = registry.get<door::State>(door);

		// collision
		// 如果门已经是开启状态,无需设置掩码
		if (state != door::State::OPENED)
		{
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

			// 只有改变了掩码才触发事件
			manager::Event::enqueue(event::door::Opened{.door = door});
		}

		// 不论门状态如何,都会播放开门动画和音效(吗?)
		std::ignore = state;

		// animation
		// 正放 -> 开门
		// 倒放 -> 关门
		// 见 blueprint::Door::sprite
		// 确保不循环
		accessor::DynamicSprite::set_mode_one_shot(registry, door);
		// 确保正向播放
		accessor::DynamicSprite::set_direction_forward(registry, door);
		// 开始播放
		accessor::DynamicSprite::replay(registry, door);

		// TODO: 音效?
	}

	auto Door::try_close(entt::registry& registry, const entt::entity door) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Door>(door));

		const auto state = registry.get<door::State>(door);

		// collision
		// 如果门已经是关闭状态,无需设置掩码
		if (state != door::State::CLOSED)
		{
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

			// 只有改变了掩码才触发事件
			manager::Event::enqueue(event::door::Closed{.door = door});
		}

		// 不论门状态如何,都会播放开门动画和音效(吗?)
		std::ignore = state;

		// animation
		// 正放 -> 开门
		// 倒放 -> 关门
		// 见 blueprint::Door::sprite
		// 确保不循环
		accessor::DynamicSprite::set_mode_one_shot(registry, door);
		// 确保反向播放
		accessor::DynamicSprite::set_direction_backward(registry, door);
		// 开始播放
		accessor::DynamicSprite::replay(registry, door);

		// TODO: 音效?
	}
}
