// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <game/constants.hpp>

#include <blueprint/animation.hpp>

#include <entt/entity/registry.hpp>

namespace pd::ctx
{
	// 动画
	class Animations final
	{
		using animations_type = blueprint::animations_type;

	public:
		static auto emplace(entt::registry& registry) noexcept -> animations_type&
		{
			return registry.ctx().emplace_as<animations_type>(Constant::blueprint_animation);
		}

		[[nodiscard]] static auto get(entt::registry& registry) noexcept -> animations_type&
		{
			return registry.ctx().get<animations_type>(Constant::blueprint_animation);
		}
	};
}
