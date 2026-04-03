// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <game/constants.hpp>

#include <utility/fixed_matrix.hpp>

namespace pd::components::floor
{
	// 房间类型
	enum class RoomType : std::uint8_t
	{
		// 该位置未生成房间
		NONE,

		// 起始房间
		START,
		// 标准房间
		STANDARD,
		// BOSS房间
		BOSS,
		// BOSS房间门钥匙房间
		KEY,
		// 商人房间
		MERCHANT,
		// 奖励房间
		BONUS,
		// 出口房间
		EXIT,
	};

	// 房间信息
	class RoomInfo final
	{
	public:
		// 房间类型
		RoomType type;
		// 此房间是否已访问过
		bool visited;
		// 此房间是否已清理过
		bool cleared;
	};

	// 房间实体(用于控制实体禁用/激活)
	class RoomEntity
	{
	public:
		std::vector<entt::entity> tiles;
		std::vector<entt::entity> doors;
		std::vector<entt::entity> chests;
		std::vector<entt::entity> destroyable_objects;
		std::vector<entt::entity> items;
		std::vector<entt::entity> corpses;
		std::vector<entt::entity> blood_stains;
	};

	// ==========================================
	// 上下文组件
	// ==========================================

	// 当前层级所有房间信息
	class RoomInfos final : public utility::FixedMatrix<RoomInfo, game::FloorHorizontalRoom, game::FloorVerticalRoom> {};

	// 当前层级所有房间的实体
	class RoomEntities final : public utility::FixedMatrix<RoomEntity, game::FloorHorizontalRoom, game::FloorVerticalRoom> {};

	// 当前生成房间数量
	class RoomCount final
	{
	public:
		std::uint32_t count;
	};

	// 当前所在房间坐标
	class CurrentRoom final
	{
	public:
		std::uint32_t x;
		std::uint32_t y;
	};

	// ==========================================
	// 实体组件
	// ==========================================

	//
}
