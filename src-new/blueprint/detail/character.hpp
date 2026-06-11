// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/animation.hpp>
#include <blueprint/detail/collision.hpp>
#include <blueprint/detail/property.hpp>
#include <blueprint/detail/name.hpp>

namespace pd::blueprint
{
	// 角色
	class Character final
	{
	public:
		// 初始位置
		Position position;

		// 动画
		Animation animation;

		// 碰撞体
		Collision collision;

		// 属性
		Property property;

		// 名字
		Name name;
	};
}
