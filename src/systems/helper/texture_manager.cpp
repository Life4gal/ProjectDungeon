// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/texture_manager.hpp>

#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto TextureManager::load(entt::registry& registry, const std::filesystem::path& path) noexcept -> entt::id_type
	{
		auto& texture_manager = registry.ctx().get<asset::TextureManager>();

		return texture_manager.load(path);
	}

	auto TextureManager::unload(entt::registry& registry, const std::filesystem::path& path) noexcept -> bool
	{
		auto& texture_manager = registry.ctx().get<asset::TextureManager>();

		return texture_manager.erase(path);
	}

	auto TextureManager::get(entt::registry& registry, const std::filesystem::path& path) noexcept -> result_type
	{
		const auto& texture_manager = registry.ctx().get<const asset::TextureManager>();

		return texture_manager.get(path);
	}
}
