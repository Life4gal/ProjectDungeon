// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/core/hashed_string.hpp>
#include <entt/resource/cache.hpp>

namespace pd::asset
{
	enum class AssetId : entt::id_type {};

	[[nodiscard]] constexpr auto to_id(const AssetId id) noexcept -> entt::id_type
	{
		return std::to_underlying(id);
	}

	[[nodiscard]] constexpr auto from_id(const entt::id_type id) noexcept -> AssetId
	{
		return static_cast<AssetId>(id);
	}

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
		[[nodiscard]] static auto make_id(const std::string_view path) noexcept -> entt::id_type
		{
			return entt::hashed_string{path.data(), path.size()};
		}

	public:
		[[nodiscard]] auto load(const std::string_view path) -> AssetId
		{
			const auto id = make_id(path);
			[[maybe_unused]] const auto [it, loaded] = cache_.load(id, path);

			return from_id(id);
		}

		auto force_load(const std::string_view path) -> AssetId
		{
			const auto id = make_id(path);
			[[maybe_unused]] const auto [it, loaded] = cache_.force_load(id, path);

			return from_id(id);
		}

	private:
		[[nodiscard]] auto get(const entt::id_type id) const noexcept -> result_type
		{
			return cache_[id];
		}

	public:
		[[nodiscard]] auto get(const AssetId id) const noexcept -> result_type
		{
			return get(to_id(id));
		}

		[[nodiscard]] auto get(const std::string_view path) const noexcept -> result_type
		{
			const auto id = make_id(path);

			return get(id);
		}

		[[nodiscard]] auto empty() const noexcept -> bool
		{
			return cache_.empty();
		}

		[[nodiscard]] auto size() const noexcept -> size_type
		{
			return cache_.size();
		}

	private:
		[[nodiscard]] auto contains(const entt::id_type id) const noexcept -> bool
		{
			return cache_.contains(id);
		}

	public:
		[[nodiscard]] auto contains(const AssetId id) const noexcept -> bool
		{
			return contains(to_id(id));
		}

		[[nodiscard]] auto contains(const std::string_view path) const noexcept -> bool
		{
			const auto id = make_id(path);

			return contains(id);
		}

	private:
		auto erase(const entt::id_type id) noexcept -> bool
		{
			return cache_.erase(id) != 0;
		}

	public:
		auto erase(const AssetId id) noexcept -> bool
		{
			return erase(to_id(id));
		}

		auto erase(const std::string_view path) noexcept -> bool
		{
			const auto id = make_id(path);

			return erase(id);
		}

		auto clear() noexcept -> void
		{
			cache_.clear();
		}
	};
}
