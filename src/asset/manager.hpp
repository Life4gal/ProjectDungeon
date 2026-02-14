// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <asset/loader.hpp>

namespace pd::asset
{
	template<typename Loader>
	class Manager
	{
	public:
		using resource_type = Loader::result_type::element_type;
		using cache_type = entt::resource_cache<resource_type, Loader>;

		using result_type = entt::resource<const resource_type>;

	private:
		cache_type cache_;

	public:
		template<std::derived_from<map_type> Map>
		auto load(const Map& map) noexcept -> void
		{
			for (const auto& [id, path]: map)
			{
				cache_.force_load(id, path);
			}
		}

		[[nodiscard]] auto get(const entt::id_type id) const noexcept -> result_type
		{
			return cache_[id];
		}
	};

	class FontManager final : public Manager<FontLoader>
	{
		using Manager::Manager;
	};

	class TextureManager final : public Manager<TextureLoader>
	{
		using Manager::Manager;
	};

	class SoundManager final : public Manager<SoundLoader>
	{
		using Manager::Manager;
	};

	class MusicManager final : public Manager<MusicLoader>
	{
		using Manager::Manager;
	};
}
