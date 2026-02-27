// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <entt/fwd.hpp>

namespace pd::config
{
	class Dungeon;
}

namespace pd::systems::helper
{
	class Dungeon final
	{
	public:
		// 加载地下城
		static auto load(entt::registry& registry, const std::filesystem::path& path) noexcept -> bool;

		// 加载地下城
		static auto load(entt::registry& registry, config::Dungeon dungeon) noexcept -> bool;

		// 卸载地下城
		static auto unload(entt::registry& registry) noexcept -> void;
	};
}
