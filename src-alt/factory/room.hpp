// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/def.hpp>

#include <entt/fwd.hpp>

namespace pd::factory
{
	class Room final
	{
	public:
		using type_type = blueprint::RoomType;

		struct position_type
		{
			std::uint32_t x;
			std::uint32_t y;
		};

		// 创建房间
		static auto spawn(entt::registry& registry, type_type type, position_type position) noexcept -> entt::entity;

		// 销毁房间
		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 销毁所有房间
		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
