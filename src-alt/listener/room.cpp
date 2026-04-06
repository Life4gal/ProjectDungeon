// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/room.hpp>

#include <manager/event.hpp>

#include <events/door.hpp>
#include <events/chest.hpp>

#include <components/floor.hpp>

#include <entt/entt.hpp>

namespace pd::listener
{
	auto Room::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<room::Active, &on_active_room>(registry);
		Event::subscribe<room::Cleared, &on_clear_room>(registry);
		Event::subscribe<room::Inactive, &on_inactive_room>(registry);
		Event::subscribe<door::Contacted, &on_contact_door>(registry);
		Event::subscribe<door::SensorContacted, &on_contact_door_sensor>(registry);
	}

	auto Room::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<room::Active, &on_active_room>(registry);
		Event::unsubscribe<room::Cleared, &on_clear_room>(registry);
		Event::unsubscribe<room::Inactive, &on_inactive_room>(registry);
		Event::unsubscribe<door::Contacted, &on_contact_door>(registry);
		Event::unsubscribe<door::SensorContacted, &on_contact_door_sensor>(registry);
	}

	auto Room::on_active_room([[maybe_unused]] entt::registry& registry, const events::room::Active& event) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		// 激活当前房间的实体
		{
			const auto& entities = event.entity.get();
			const auto& [tiles, doors, chests, destroyable_objects, items, corpses, blood_stains] = entities;

			Event::enqueue(door::EnableAll{doors});
			Event::enqueue(chest::EnableAll{chests});
			// Event::enqueue(destroyable_object::EnableAll{destroyable_objects});
			// Event::enqueue(item::EnableAll{items});
			// Event::enqueue(corpse::EnableAll{corpses});
			// Event::enqueue(blood_stain::EnableAll{blood_stains});
		}

		// 如果房间未清除
		if (const auto& info = event.info.get();
			not info.cleared)
		{
			//  请求关闭所有门(如果可以关闭)
			Event::enqueue(door::CloseRequest{});
		}
	}

	auto Room::on_clear_room([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::room::Cleared& event) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		// 请求开启所有门(如果可以开启)
		Event::enqueue(door::OpenRequest{});
	}

	auto Room::on_inactive_room([[maybe_unused]] entt::registry& registry, const events::room::Inactive& event) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		// 禁用当前房间的实体
		{
			const auto& entities = event.entity.get();
			const auto& [tiles, doors, chests, destroyable_objects, items, corpses, blood_stains] = entities;

			Event::enqueue(door::DisableAll{doors});
			Event::enqueue(chest::DisableAll{chests});
			// Event::enqueue(destroyable_object::DisableAll{destroyable_objects});
			// Event::enqueue(item::DisableAll{items});
			// Event::enqueue(corpse::DisableAll{corpses});
			// Event::enqueue(blood_stain::DisableAll{blood_stains});
		}

		if (const auto& info = event.info.get();
			not info.cleared)
		{
			// 理论上不可能没有清理
		}
	}

	auto Room::on_contact_door([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::door::Contacted& event) noexcept -> void
	{
		// 房间本身应该是不在乎这个事件的
		// 或者说接触事件不在这里处理
	}

	auto Room::on_contact_door_sensor([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::door::SensorContacted& event) noexcept -> void
	{
		// 房间本身应该是不在乎这个事件的
		// 或者说接触事件不在这里处理
	}
}
