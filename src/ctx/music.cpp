// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <ctx/music.hpp>

#include <entt/entt.hpp>

namespace pd::ctx
{
	auto Music::load(entt::registry& registry, const std::filesystem::path& path) noexcept -> entt::id_type
	{
		auto& music_manager = registry.ctx().get<asset::MusicManager>();

		return music_manager.load(path);
	}

	auto Music::get(entt::registry& registry, const std::filesystem::path& path) noexcept -> result_type
	{
		const auto& music_manager = registry.ctx().get<const asset::MusicManager>();

		return music_manager.get(path);
	}
} // namespace pd::ctx
