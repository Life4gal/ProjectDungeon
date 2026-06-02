// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/transform.hpp>
#include <component/render.hpp>
#include <component/collision.hpp>
#include <component/property.hpp>
#include <component/name.hpp>
#include <component/tags.hpp>

namespace pd::component::player
{
	// transform --> 玩家变换
	// render --> 玩家渲染
	// collision --> 玩家碰撞体
	// property --> 玩家属性
	// name --> 玩家名称
	// tags --> 玩家标签

	// 移动速度
	// FIXME(OPT): 将它移动到合适的地方
	class Speed final
	{
	public:
		float speed;
	};

	// TODO: 其他组件
}
