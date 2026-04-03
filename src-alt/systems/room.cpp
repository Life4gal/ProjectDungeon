// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/room.hpp>

#include <manager/event.hpp>

#include <components/floor.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Room::subscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<room::Active, &on_active_room>(registry);
		Event::subscribe<room::Cleared, &on_clear_room>(registry);
		Event::subscribe<room::Inactive, &on_inactive_room>(registry);

		Event::subscribe<door::Contacted, &on_contact_door>(registry);
		Event::subscribe<door::SensorContacted, &on_contact_door_sensor>(registry);
	}

	auto Room::unsubscribe_events(entt::registry& registry) noexcept -> void
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

			Event::trigger(room::PreEnableTile{tiles});
			Event::trigger(room::PreEnableDoor{doors});
			Event::trigger(room::PreEnableChest{chests});
			Event::trigger(room::PreEnableDestroyableObject{destroyable_objects});
			Event::trigger(room::PreEnableItem{items});
			Event::trigger(room::PreEnableCorpse{corpses});
			Event::trigger(room::PreEnableBloodStain{blood_stains});

			//

			Event::trigger(room::PostEnableTile{tiles});
			Event::trigger(room::PostEnableDoor{doors});
			Event::trigger(room::PostEnableChest{chests});
			Event::trigger(room::PostEnableDestroyableObject{chests});
			Event::trigger(room::PostEnableItem{items});
			Event::trigger(room::PostEnableCorpse{corpses});
			Event::trigger(room::PostEnableBloodStain{blood_stains});
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

			Event::trigger(room::PreDisableTile{tiles});
			Event::trigger(room::PreDisableDoor{doors});
			Event::trigger(room::PreDisableChest{chests});
			Event::trigger(room::PreDisableDestroyableObject{destroyable_objects});
			Event::trigger(room::PreDisableItem{items});
			Event::trigger(room::PreDisableCorpse{corpses});
			Event::trigger(room::PreDisableBloodStain{blood_stains});

			//

			Event::trigger(room::PostDisableTile{tiles});
			Event::trigger(room::PostDisableDoor{doors});
			Event::trigger(room::PostDisableChest{chests});
			Event::trigger(room::PostDisableDestroyableObject{chests});
			Event::trigger(room::PostDisableItem{items});
			Event::trigger(room::PostDisableCorpse{corpses});
			Event::trigger(room::PostDisableBloodStain{blood_stains});
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

	auto Room::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;
	}

	auto Room::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;
	}
}
