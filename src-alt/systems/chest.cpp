// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/chest.hpp>

#include <manager/event.hpp>

#include <components/chest.hpp>
#include <components/physics_world.hpp>

#include <systems/physics_world.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Chest::create(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		Event::subscribe<events::ChestContacted, &on_contact>(registry);
	}

	auto Chest::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		Event::unsubscribe<events::ChestContacted, &on_contact>(registry);
	}

	auto Chest::on_contact(entt::registry& registry, const events::ChestContacted& event) noexcept -> void
	{
		using namespace components;
		using namespace manager;

		// 其必定存在物理碰撞体
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(event.chest));
		PhysicsWorld::deattach(registry, event.chest);

		// 处理效果
		switch (const auto type = registry.get<chest::Type>(event.chest);
			type)
		{
			case chest::Type::COIN:
			{
				break;
			}
			case chest::Type::ITEM:
			{
				break;
			}
			case chest::Type::EQUIPMENT:
			{
				break;
			}
			case chest::Type::CONSUMABLE:
			{
				break;
			}
			case chest::Type::EXPLOSION_TRAP:
			{
				break;
			}
			case chest::Type::ROCKFALL_TRAP:
			{
				break;
			}
		}

		// 房间缓存由房间处理事件时修改
		// FIXME: 要不要销毁实体?
		registry.destroy(event.chest);
	}
}
