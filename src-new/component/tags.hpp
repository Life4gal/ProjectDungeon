// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

// tags和state没有依赖关系
// 但是考虑到用到tags的地方基本都会用到state
// 不如直接引入其作为依赖 :)
#include <component/state.hpp>

namespace pd::component::tags
{
	// 地下城 -- 关卡 -- 房间 -- 地板
	class Floor final {};

	// 地下城-- 关卡 -- 房间 -- 门
	class Door final {};

	// 地下城 -- 关卡 -- 房间 -- 墙壁
	class Wall final {};

	// 地下城-- 关卡 -- 房间
	class Room final {};

	// 敌人
	class Enemy final {};

	// 玩家
	class Player final {};

	// 飞弹
	class Projectile final {};
}
