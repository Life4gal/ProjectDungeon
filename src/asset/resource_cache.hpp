// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <entt/core/hashed_string.hpp>
#include <entt/resource/cache.hpp>
#include <entt/entity/entity.hpp>

namespace pd::asset
{
	template<typename Loader>
	class ResourceCache
	{
		using loader_type = Loader;
		using element_type = loader_type::result_type::element_type;

		using cache_type = entt::resource_cache<element_type, loader_type>;

	public:
		using size_type = cache_type::size_type;

		using result_type = entt::resource<const element_type>;

	private:
		cache_type cache_;

		// todo: 当前保存资源路径,所以每次查找都要进行一次ID计算
		//  后续会将相关内容替换为ID,不过当前不着急
		[[nodiscard]] static auto make_id(const std::filesystem::path& path) noexcept -> entt::id_type
		{
			const auto& s = path.native();

			return entt::hashed_wstring{s.c_str(), s.size()};
		}

	public:
		[[nodiscard]] auto load(const std::filesystem::path& path) -> entt::id_type
		{
			const auto id = make_id(path);
			[[maybe_unused]] const auto [it, loaded] = cache_.load(id, path);

			return id;
		}

		auto force_load(const std::filesystem::path& path) -> entt::id_type
		{
			const auto id = make_id(path);
			[[maybe_unused]] const auto [it, loaded] = cache_.force_load(id, path);

			return id;
		}

		[[nodiscard]] auto get(entt::id_type id) const -> result_type
		{
			return cache_[id];
		}

		[[nodiscard]] auto get(const std::filesystem::path& path) const -> result_type
		{
			return get(make_id(path));
		}

		[[nodiscard]] auto empty() const noexcept -> bool
		{
			return cache_.empty();
		}

		[[nodiscard]] auto size() const noexcept -> size_type
		{
			return cache_.size();
		}

		[[nodiscard]] auto contains(const entt::id_type id) const noexcept -> bool
		{
			return cache_.contains(id);
		}

		[[nodiscard]] auto contains(const std::filesystem::path& path) const noexcept -> bool
		{
			return contains(make_id(path));
		}

		auto erase(const entt::id_type id) noexcept -> bool
		{
			return cache_.erase(id) != 0;
		}

		auto erase(const std::filesystem::path& path) noexcept -> bool
		{
			return erase(make_id(path));
		}

		auto clear() noexcept -> void
		{
			cache_.clear();
		}
	};
}
