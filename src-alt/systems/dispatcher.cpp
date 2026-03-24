// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/dispatcher.hpp>

#include <components/game.hpp>

#include <events/game.hpp>

#include <game/game.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Dispatcher::scene_change(entt::registry& registry, const game::Scenes to) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::Game>());

		auto& [game] = registry.ctx().get<components::Game>();
		auto& dispatcher = game.get().dispatcher();

		dispatcher.enqueue(events::RequestSceneChange{.to = to});
	}
}
