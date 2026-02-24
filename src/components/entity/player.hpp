// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::components::player
{
	// animation --> 玩家的动画状态(当前动画/当前帧/动画计时器等)
	// actor --> 玩家的生命值/魔法值/速度等属性

	// transform --> 玩家的位置/缩放/旋转
	// render --> 玩家的纹理/颜色
	// physics_body --> 玩家的物理属性(关闭时阻止移动,打开时允许移动)

	class Entity final
	{
	public:
		entt::entity entity;
	};

	class Movement final
	{
	public:
		enum class Type : std::uint8_t
		{
			STAY,
			FORWARD,
			BACKWARD,
		};

		Type x;
		Type y;
	};
}
