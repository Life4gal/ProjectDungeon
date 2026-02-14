// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <systems/system.hpp>

namespace pd::systems
{
	class Debug final : public System<Debug>
	{
		friend System;

		static auto do_loaded(entt::registry& registry) noexcept -> void;

		static auto do_initialized(entt::registry& registry) noexcept -> void;

		static auto do_unloaded(entt::registry& registry) noexcept -> void;

		static auto do_update(entt::registry& registry, sf::Time delta) noexcept -> void;

		static auto do_render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;
	};
}
