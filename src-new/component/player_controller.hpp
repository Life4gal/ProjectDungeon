// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::component::player_controller
{
	// 玩家控制器仅限操作玩家实体吗?还是支持操作任意实体?

	class Target final
	{
	public:
		entt::entity entity;
	};

	enum class MovementType : std::int8_t
	{
		// 后退
		BACKWARD = -1,
		// 停止
		STAY = 0,
		// 前进
		FORWARD = 1,
	};

	class HorizontalMovement final
	{
	public:
		MovementType type;
	};

	class VerticalMovement final
	{
	public:
		MovementType type;
	};
}
