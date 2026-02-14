// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <utility/map.hpp>

#include <entt/core/hashed_string.hpp>

namespace pd::asset
{
	using path_type = std::filesystem::path;

	constexpr auto path_to_id = [](const path_type& path) noexcept -> entt::id_type
	{
		const auto& s = path.native();

		return entt::hashed_wstring{s.c_str(), s.size()};
	};

	using map_type = utility::Map<path_to_id, path_type>;

	class FontMap final : public map_type
	{
	public:
		using Map::Map;
	};

	class TextureMap final : public map_type
	{
	public:
		using Map::Map;
	};

	class SoundMap final : public map_type
	{
	public:
		using Map::Map;
	};

	class MusicMap final : public map_type
	{
	public:
		using Map::Map;
	};
}
