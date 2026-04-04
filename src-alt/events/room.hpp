// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>
#include <functional>

#include <entt/entity/fwd.hpp>

namespace pd::components::floor
{
	class RoomInfo;
	class RoomEntity;
}

namespace pd::events::room
{
	// 激活(进入)房间
	class Active final
	{
	public:
		std::reference_wrapper<const components::floor::RoomInfo> info;
		std::reference_wrapper<const components::floor::RoomEntity> entity;
	};

	// 清理房间
	class Cleared final
	{
	public:
		std::reference_wrapper<const components::floor::RoomInfo> info;
		std::reference_wrapper<const components::floor::RoomEntity> entity;
	};

	// 禁用(离开)房间
	class Inactive final
	{
	public:
		std::reference_wrapper<const components::floor::RoomInfo> info;
		std::reference_wrapper<const components::floor::RoomEntity> entity;
	};

	namespace detail
	{
		class Entities
		{
		public:
			std::span<const entt::entity> entities;
		};
	}

	// 禁用当前房间瓦片实体
	class DisableTile final : public detail::Entities {};

	// 启用当前房间瓦片实体
	class EnableTile final : public detail::Entities {};

	// 禁用当前房间门实体
	class DisableDoor final : public detail::Entities {};

	// 启用当前房间门实体
	class EnableDoor final : public detail::Entities {};

	// 禁用当前房间宝箱实体
	class DisableChest final : public detail::Entities {};

	// 启用当前房间宝箱实体
	class EnableChest final : public detail::Entities {};

	// 禁用当前房间可破坏物实体
	class DisableDestroyableObject final : public detail::Entities {};

	// 启用当前房间可破坏物实体
	class EnableDestroyableObject final : public detail::Entities {};

	// 禁用当前房间物品实体
	class DisableItem final : public detail::Entities {};

	// 启用当前房间物品实体
	class EnableItem final : public detail::Entities {};

	// 禁用当前房间尸体实体
	class DisableCorpse final : public detail::Entities {};

	// 启用当前房间尸体实体
	class EnableCorpse final : public detail::Entities {};

	// 禁用当前房间血迹实体
	class DisableBloodStain final : public detail::Entities {};

	// 启用当前房间血迹实体
	class EnableBloodStain final : public detail::Entities {};
}
