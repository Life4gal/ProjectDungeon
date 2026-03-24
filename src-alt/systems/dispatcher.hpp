// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <game/scene.hpp>

#include <entt/fwd.hpp>

namespace pd::systems
{
	class Dispatcher final
	{
	public:
		// 场景切换
		static auto scene_change(entt::registry& registry, game::Scenes to) noexcept -> void;
	};
}
