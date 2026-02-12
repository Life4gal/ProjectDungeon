// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/world.hpp>

#include <components/world.hpp>

#include <entt/entt.hpp>

namespace pd::systems::update
{
	auto world(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;

		auto& [frame_delta] = registry.ctx().get<FrameDelta>();
		auto& [elapsed] = registry.ctx().get<SceneElapsedTime>();

		frame_delta = delta;
		elapsed += delta;
	}

	auto world_real(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;

		auto& [elapsed] = registry.ctx().get<SceneRealElapsedTime>();

		elapsed += delta;
	}
}
