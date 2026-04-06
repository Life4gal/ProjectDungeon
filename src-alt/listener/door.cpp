// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/door.hpp>

#include <scene/game.hpp>

#include <manager/event.hpp>

#include <components/tags.hpp>
#include <components/transform.hpp>

#include <attacher/physics_body.hpp>

#include <factory/door.hpp>

#include <entt/entt.hpp>

namespace pd::listener
{
	auto Door::do_disable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components;

		registry.emplace_or_replace<tags::disabled>(entity);

		// 禁用时销毁物理体
		// FIXME(OPT): 禁用物理体?
		attacher::PhysicsBody::deattach(registry, entity);
	}

	auto Door::do_enable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components;

		registry.remove<tags::disabled>(entity);

		// 启用时创建物理体
		// FIXME(OPT): 重新启用物理体?

		const auto* position = registry.try_get<transform::Position>(entity);
		if (position == nullptr)
		{
			return;
		}

		const auto* direction = registry.try_get<door::Direction>(entity);
		if (direction == nullptr)
		{
			return;
		}

		factory::Door::create_physics_body(registry, entity, position->position, *direction);
	}

	auto Door::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<door::Disable, &Door::on_disable>(registry);
		Event::subscribe<door::Enable, &Door::on_enable>(registry);
		Event::subscribe<door::DisableAll, &Door::on_disable_all>(registry);
		Event::subscribe<door::EnableAll, &Door::on_enable_all>(registry);
		Event::subscribe<door::OpenRequest, &Door::on_open_request>(registry);
		Event::subscribe<door::CloseRequest, &Door::on_close_request>(registry);
		Event::subscribe<door::Contacted, &Door::on_contact>(registry);
		Event::subscribe<door::SensorContacted, &Door::on_contact_sensor>(registry);
	}

	auto Door::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<door::Disable, &Door::on_disable>(registry);
		Event::unsubscribe<door::Enable, &Door::on_enable>(registry);
		Event::unsubscribe<door::DisableAll, &Door::on_disable_all>(registry);
		Event::unsubscribe<door::EnableAll, &Door::on_enable_all>(registry);
		Event::unsubscribe<door::OpenRequest, &Door::on_open_request>(registry);
		Event::unsubscribe<door::CloseRequest, &Door::on_close_request>(registry);
		Event::unsubscribe<door::Contacted, &Door::on_contact>(registry);
		Event::unsubscribe<door::SensorContacted, &Door::on_contact_sensor>(registry);
	}

	auto Door::on_disable(entt::registry& registry, const events::door::Disable& event) noexcept -> void
	{
		do_disable(registry, event.door);
	}

	auto Door::on_enable(entt::registry& registry, const events::door::Enable& event) noexcept -> void
	{
		do_enable(registry, event.door);
	}

	auto Door::on_disable_all(entt::registry& registry, const events::door::DisableAll& event) noexcept -> void
	{
		for (const auto door: event.doors)
		{
			do_disable(registry, door);
		}
	}

	auto Door::on_enable_all(entt::registry& registry, const events::door::EnableAll& event) noexcept -> void
	{
		for (const auto door: event.doors)
		{
			do_enable(registry, door);
		}
	}

	auto Door::on_open_request(entt::registry& registry, const events::door::OpenRequest& event) noexcept -> void
	{
		using namespace components;

		for (const auto view = registry.view<door::State>(entt::exclude<tags::disabled>);
		     const auto [entity, state]: view.each())
		{
			if (state == door::State::OPENING or state == door::State::OPENED)
			{
				continue;
			}

			state = door::State::OPENING;
			registry.emplace_or_replace<door::Timer>(entity, sf::Time::Zero);
		}
	}

	auto Door::on_close_request(entt::registry& registry, const events::door::CloseRequest& event) noexcept -> void
	{
		using namespace components;

		for (const auto view = registry.view<door::State>(entt::exclude<tags::disabled>);
		     const auto [entity, state]: view.each())
		{
			if (state == door::State::CLOSING or state == door::State::CLOSED)
			{
				continue;
			}

			state = door::State::CLOSING;
			registry.emplace_or_replace<door::Timer>(entity, sf::Time::Zero);
		}
	}

	auto Door::on_contact([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::door::Contacted& event) noexcept -> void
	{
		// 门本身应该是不在乎这个事件的
		// 或者说接触事件不在这里处理
		// todo: 门应该在乎这个事件,只是我们可能需要足够的信息才能处理?
	}

	auto Door::on_contact_sensor([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::door::SensorContacted& event) noexcept -> void
	{
		// 门本身应该是不在乎这个事件的
		// 或者说接触事件不在这里处理
		// todo: 门应该在乎这个事件,只是我们可能需要足够的信息才能处理?
	}
}
