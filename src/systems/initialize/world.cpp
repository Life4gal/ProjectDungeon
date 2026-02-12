// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/world.hpp>

#include <components/world.hpp>

#include <entt/entt.hpp>

namespace pd::systems::initialize
{
	auto world(entt::registry& registry) noexcept -> void
	{
		registry.ctx().emplace<components::FrameDelta>(sf::seconds(1));
		registry.ctx().emplace<components::SceneElapsedTime>(sf::Time::Zero);
		registry.ctx().emplace<components::SceneRealElapsedTime>(sf::Time::Zero);
	}
}
