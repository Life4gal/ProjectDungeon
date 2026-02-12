// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <utility/map.hpp>

#include <entt/core/hashed_string.hpp>
#include <entt/resource/cache.hpp>

namespace pd::asset
{
	using path_type = std::filesystem::path;

	constexpr auto path_to_id = [](const path_type& path) noexcept -> entt::id_type
	{
		const auto& s = path.native();

		return entt::hashed_wstring{s.c_str(), s.size()};
	};

	using map_type = utility::Map<path_to_id, path_type>;

	template<typename Loader>
	class AssetLoader
	{
	public:
		using resource_type = Loader::result_type::element_type;
		using cache_type = entt::resource_cache<resource_type, Loader>;
		using result_type = entt::resource<const resource_type>;

		// ctx::Map依赖此别名,定义该别名可以使ctx::Map更加通用(而不是直接以来asset::map_type)
		using map_type = map_type;

	private:
		cache_type cache_;

	public:
		auto load(const map_type& map) noexcept -> void
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
}
