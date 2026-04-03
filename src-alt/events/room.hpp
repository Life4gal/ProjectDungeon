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

	// 禁用当前房间瓦片实体前*立刻*触发
	class PreDisableTile final : public detail::Entities {};

	// 禁用当前房间瓦片实体后*立刻*触发
	class PostDisableTile final : public detail::Entities {};

	// 启用当前房间瓦片实体前*立刻*触发
	class PreEnableTile final : public detail::Entities {};

	// 启用当前房间瓦片实体后*立刻*触发
	class PostEnableTile final : public detail::Entities {};

	// 禁用当前房间门实体前*立刻*触发
	class PreDisableDoor final : public detail::Entities {};

	// 禁用当前房间门实体后*立刻*触发
	class PostDisableDoor final : public detail::Entities {};

	// 启用当前房间门实体前*立刻*触发
	class PreEnableDoor final : public detail::Entities {};

	// 启用当前房间门实体后*立刻*触发
	class PostEnableDoor final : public detail::Entities {};

	// 禁用当前房间宝箱实体前*立刻*触发
	class PreDisableChest final : public detail::Entities {};

	// 禁用当前房间宝箱实体后*立刻*触发
	class PostDisableChest final : public detail::Entities {};

	// 启用当前房间宝箱实体前*立刻*触发
	class PreEnableChest final : public detail::Entities {};

	// 启用当前房间宝箱实体后*立刻*触发
	class PostEnableChest final : public detail::Entities {};

	// 禁用当前房间可破坏物实体前*立刻*触发
	class PreDisableDestroyableObject final : public detail::Entities {};

	// 禁用当前房间可破坏物实体后*立刻*触发
	class PostDisableDestroyableObject final : public detail::Entities {};

	// 启用当前房间可破坏物实体前*立刻*触发
	class PreEnableDestroyableObject final : public detail::Entities {};

	// 启用当前房间可破坏物实体后*立刻*触发
	class PostEnableDestroyableObject final : public detail::Entities {};

	// 禁用当前房间物品实体前*立刻*触发
	class PreDisableItem final : public detail::Entities {};

	// 禁用当前房间物品实体后*立刻*触发
	class PostDisableItem final : public detail::Entities {};

	// 启用当前房间物品实体前*立刻*触发
	class PreEnableItem final : public detail::Entities {};

	// 启用当前房间物品实体后*立刻*触发
	class PostEnableItem final : public detail::Entities {};

	// 禁用当前房间尸体实体前*立刻*触发
	class PreDisableCorpse final : public detail::Entities {};

	// 禁用当前房间尸体实体后*立刻*触发
	class PostDisableCorpse final : public detail::Entities {};

	// 启用当前房间尸体实体前*立刻*触发
	class PreEnableCorpse final : public detail::Entities {};

	// 启用当前房间尸体实体后*立刻*触发
	class PostEnableCorpse final : public detail::Entities {};

	// 禁用当前房间血迹实体前*立刻*触发
	class PreDisableBloodStain final : public detail::Entities {};

	// 禁用当前房间血迹实体后*立刻*触发
	class PostDisableBloodStain final : public detail::Entities {};

	// 启用当前房间血迹实体前*立刻*触发
	class PreEnableBloodStain final : public detail::Entities {};

	// 启用当前房间血迹实体后*立刻*触发
	class PostEnableBloodStain final : public detail::Entities {};
}
