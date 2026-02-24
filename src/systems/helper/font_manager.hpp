// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <asset/font.hpp>

#include <entt/fwd.hpp>

namespace pd::systems::helper
{
	class FontManager final
	{
	public:
		using result_type = asset::FontManager::result_type;

		static auto load(entt::registry& registry, const std::filesystem::path& path) noexcept -> entt::id_type;

		[[nodiscard]] static auto get(entt::registry& registry, const std::filesystem::path& path) noexcept -> result_type;
	};
}
