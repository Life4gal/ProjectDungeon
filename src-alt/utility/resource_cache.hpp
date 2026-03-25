// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility/resource_cache_fwd.hpp>

#include <entt/core/hashed_string.hpp>
#include <entt/resource/cache.hpp>

namespace pd::utility
{
	namespace detail
	{
		[[nodiscard]] constexpr auto to_id(const AssetId id) noexcept -> entt::id_type
		{
			return std::to_underlying(id);
		}

		[[nodiscard]] constexpr auto from_id(const entt::id_type id) noexcept -> AssetId
		{
			return static_cast<AssetId>(id);
		}

		[[nodiscard]] constexpr auto make_id(const std::string_view path) noexcept -> entt::id_type
		{
			return entt::hashed_string{path.data(), path.size()};
		}
	}

	template<typename Loader>
	class ResourceCache
	{
		using loader_type = Loader;
		using element_type = Loader::result_type::element_type;
		using cache_type = entt::resource_cache<element_type, loader_type>;

	public:
		using reference = entt::resource<element_type>;
		using const_reference = entt::resource<const element_type>;

		using size_type = cache_type::size_type;

	private:
		cache_type cache_;

	public:
		[[nodiscard]] auto load(const std::string_view path) noexcept -> AssetId
		{
			const auto id = detail::make_id(path);
			[[maybe_unused]] const auto [it, loaded] = cache_.load(id, path);

			return detail::from_id(id);
		}

		[[nodiscard]] auto force_load(const std::string_view path) -> AssetId
		{
			const auto id = detail::make_id(path);
			[[maybe_unused]] const auto [it, loaded] = cache_.force_load(id, path);

			return detail::from_id(id);
		}

	private:
		[[nodiscard]] auto get(const entt::id_type id) noexcept -> reference
		{
			return cache_[id];
		}

		[[nodiscard]] auto get(const entt::id_type id) const noexcept -> const_reference
		{
			return cache_[id];
		}

	public:
		[[nodiscard]] auto get(const AssetId id) noexcept -> reference
		{
			return get(detail::to_id(id));
		}

		[[nodiscard]] auto get(const std::string_view path) noexcept -> reference
		{
			const auto id = detail::make_id(path);

			return get(id);
		}

		[[nodiscard]] auto get(const AssetId id) const noexcept -> const_reference
		{
			return get(detail::to_id(id));
		}

		[[nodiscard]] auto get(const std::string_view path) const noexcept -> const_reference
		{
			const auto id = detail::make_id(path);

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
			return contains(detail::to_id(id));
		}

		[[nodiscard]] auto contains(const std::string_view path) const noexcept -> bool
		{
			const auto id = detail::make_id(path);

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
			return erase(detail::to_id(id));
		}

		auto erase(const std::string_view path) noexcept -> bool
		{
			const auto id = detail::make_id(path);

			return erase(id);
		}

		auto clear() noexcept -> void
		{
			cache_.clear();
		}
	};
}
