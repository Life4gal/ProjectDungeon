// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility/map.hpp>

#include <entt/entity/registry.hpp>

namespace pd::ctx
{
	template<typename T>
		requires requires
		{
			T::name;
		}
	class Map final
	{
		using map_type = T::map_type;

	public:
		using key_type = map_type::key_type;
		using mapping_type = map_type::mapping_type;

		using kv_type = map_type::kv_type;
		using vk_type = map_type::vk_type;

		using view_type = map_type::view_type;
		using ids_type = map_type::ids_type;
		using paths_type = map_type::paths_type;

		constexpr static auto name = T::name;

		static auto emplace(entt::registry& registry) noexcept -> map_type&
		{
			return registry.ctx().emplace_as<map_type>(name);
		}

		[[nodiscard]] static auto get(entt::registry& registry) noexcept -> map_type&
		{
			return registry.ctx().get<map_type>(name);
		}

		[[nodiscard]] static auto id_of(entt::registry& registry, const mapping_type& value) noexcept -> key_type
		{
			return get(registry).id_of(value);
		}

		[[nodiscard]] static auto id_of(entt::registry& registry, mapping_type&& value) noexcept -> key_type
		{
			return get(registry).id_of(std::move(value));
		}

		[[nodiscard]] static auto path_of(entt::registry& registry, const key_type id) noexcept -> const mapping_type*
		{
			return get(registry).path_of(id);
		}

		[[nodiscard]] static auto begin(entt::registry& registry) noexcept -> kv_type::const_iterator
		{
			return get(registry).begin();
		}

		[[nodiscard]] static auto end(entt::registry& registry) noexcept -> kv_type::const_iterator
		{
			return get(registry).end();
		}

		[[nodiscard]] static auto view(entt::registry& registry) noexcept -> view_type
		{
			return get(registry).view();
		}

		[[nodiscard]] static auto ids(entt::registry& registry) noexcept -> ids_type
		{
			return get(registry).ids();
		}

		[[nodiscard]] static auto paths(entt::registry& registry) noexcept -> paths_type
		{
			return get(registry).paths();
		}
	};
}
