// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <unordered_map>
#include <ranges>

#include <entt/entity/entity.hpp>

namespace pd::utility
{
	template<auto IdMaker, typename ValueType, typename Hash= std::hash<ValueType>, typename Comparator = std::ranges::equal_to>
		requires requires
		{
			{ IdMaker(std::declval<const ValueType&>()) } -> std::same_as<entt::id_type>;
		}
	class Map final
	{
	public:
		using key_type = entt::id_type;
		using mapping_type = ValueType;

		using kv_type = std::unordered_map<key_type, mapping_type>;
		using vk_type = std::unordered_map<std::reference_wrapper<const mapping_type>, key_type, Hash, Comparator>;

		using view_type = std::ranges::ref_view<const kv_type>;
		using ids_type = std::ranges::keys_view<view_type>;
		using paths_type = std::ranges::values_view<view_type>;

	private:
		kv_type id_to_value_;
		vk_type value_to_id_;

		[[nodiscard]] auto search(const mapping_type& value) const noexcept -> key_type
		{
			if (const auto it = value_to_id_.find(value);
				it != value_to_id_.end())
			{
				return it->second;
			}

			return entt::null;
		}

		[[nodiscard]] auto create(mapping_type&& value) noexcept -> key_type
		{
			const auto id = IdMaker(value);
			const auto [it, _] = id_to_value_.emplace(id, std::move(value));
			value_to_id_.emplace(it->second, it->first);

			return id;
		}

	public:
		[[nodiscard]] auto id_of(const mapping_type& value) noexcept -> key_type
		{
			if (const auto id = search(value);
				id != entt::null)
			{
				return id;
			}

			return create(auto{value});
		}

		[[nodiscard]] auto id_of(mapping_type&& value) noexcept -> key_type
		{
			if (const auto id = search(value);
				id != entt::null)
			{
				return id;
			}

			return create(std::move(value));
		}

		[[nodiscard]] auto value_of(key_type id) const noexcept -> const mapping_type*
		{
			if (const auto it = id_to_value_.find(id);
				it != id_to_value_.end())
			{
				return &it->second;
			}

			return nullptr;
		}

		[[nodiscard]] auto begin() const noexcept -> kv_type::const_iterator
		{
			return id_to_value_.begin();
		}

		[[nodiscard]] auto end() const noexcept -> kv_type::const_iterator
		{
			return id_to_value_.end();
		}

		[[nodiscard]] auto view() const noexcept -> view_type
		{
			return id_to_value_ | std::views::all;
		}

		[[nodiscard]] auto ids() const noexcept -> ids_type
		{
			return id_to_value_ | std::views::keys;
		}

		[[nodiscard]] auto values() const noexcept -> paths_type
		{
			return id_to_value_ | std::views::values;
		}
	};
}
