// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/font_manager.hpp>

#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto FontManager::load(entt::registry& registry, const std::filesystem::path& path) noexcept -> entt::id_type
	{
		auto& font_manager = registry.ctx().get<asset::FontManager>();

		return font_manager.load(path);
	}

	auto FontManager::get(entt::registry& registry, const std::filesystem::path& path) noexcept -> result_type
	{
		const auto& font_manager = registry.ctx().get<const asset::FontManager>();

		return font_manager.get(path);
	}
}
