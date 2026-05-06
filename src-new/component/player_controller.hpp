// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::component::player_controller
{
	// 玩家控制器仅限操作玩家实体吗?还是支持操作任意实体?

	// 玩家操作器当前操作的实体
	// FIXME: 当前我们只检查Target组件是否存在(如果存在则假定目标实体合法)
	class Target final
	{
	public:
		entt::entity entity;
	};

	// 移动类型(方向)
	// TODO: 支持更丰富的操作
	enum class MovementType : std::int8_t
	{
		// 后退
		BACKWARD = -1,
		// 停止
		STAY = 0,
		// 前进
		FORWARD = 1,
	};

	// 水平移动
	class HorizontalMovement final
	{
	public:
		MovementType type;
	};

	// 纵向移动
	class VerticalMovement final
	{
	public:
		MovementType type;
	};
}
