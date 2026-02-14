// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/world.hpp>

#include <components/world.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems
{
	auto World::do_loaded(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		// 避免可能的除0,初始delta设置为非0值
		registry.ctx().emplace<FrameDelta>(sf::seconds(1));
		registry.ctx().emplace<TotalElapsed>(sf::Time::Zero);
		registry.ctx().emplace<PlayElapsed>(sf::Time::Zero);

		// 创建物理世界
		{
			auto def = b2DefaultWorldDef();
			// 无重力世界
			def.gravity = b2Vec2_zero;

			registry.ctx().emplace<PhysicsWorld>(b2CreateWorld(&def));
		}
	}

	auto World::do_initialized([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto World::do_unloaded(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		// 销毁物理世界
		{
			const auto [world_id] = registry.ctx().get<PhysicsWorld>();
			b2DestroyWorld(world_id);
		}
	}

	auto World::do_update(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;

		auto& [frame_delta] = registry.ctx().get<FrameDelta>();
		auto& [total_elapsed] = registry.ctx().get<TotalElapsed>();

		frame_delta = delta;
		total_elapsed += delta;

		if (not is_pause(registry))
		{
			auto& [play_elapsed] = registry.ctx().get<PlayElapsed>();

			play_elapsed += delta;
		}
	}

	auto World::do_render([[maybe_unused]] entt::registry& registry, [[maybe_unused]] sf::RenderWindow& window) noexcept -> void
	{
		// 什么也不做
		// 也许可以输出一些信息,例如帧数?
	}

	auto World::frame_delta(entt::registry& registry) noexcept -> sf::Time
	{
		return registry.ctx().get<components::FrameDelta>().frame_delta;
	}

	auto World::total_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		return registry.ctx().get<components::TotalElapsed>().total_elapsed;
	}

	auto World::play_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		return registry.ctx().get<components::PlayElapsed>().play_elapsed;
	}

	auto World::pause(entt::registry& registry) noexcept -> void
	{
		registry.ctx().emplace<components::GamePaused>();
	}

	auto World::unpause(entt::registry& registry) noexcept -> void
	{
		registry.ctx().erase<components::GamePaused>();
	}

	auto World::is_pause(entt::registry& registry) noexcept -> bool
	{
		return registry.ctx().contains<components::GamePaused>();
	}

	auto World::physics_world(entt::registry& registry) noexcept -> b2WorldId
	{
		return registry.ctx().get<components::PhysicsWorld>().world_id;
	}
}
