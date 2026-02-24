// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/sound_manager.hpp>

#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto SoundManager::load(entt::registry& registry, const std::filesystem::path& path) noexcept -> entt::id_type
	{
		auto& sound_manager = registry.ctx().get<asset::SoundManager>();

		return sound_manager.load(path);
	}

	auto SoundManager::get(entt::registry& registry, const std::filesystem::path& path) noexcept -> result_type
	{
		const auto& sound_manager = registry.ctx().get<const asset::SoundManager>();

		return sound_manager.get(path);
	}
}
