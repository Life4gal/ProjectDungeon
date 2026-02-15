// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::components::door
{
	// transform --> 门的位置/缩放/旋转
	// render --> 门的纹理/颜色
	// physics_body --> 门的物理属性(关闭时阻止移动,打开时允许移动)

	// 标记当前门是否锁住
	// 如果未锁住则不存在该数据
	class Locked final {};

	// 标记当前门的钥匙
	// 如果是null则表示不需要钥匙
	// 即使有钥匙也需要消灭当前房间所有敌人才能解锁门
	// 也就是说需要同时满足拥有钥匙(如果需要)和消灭敌人这两个条件才能解锁门
	class Key final
	{
	public:
		entt::entity key;
	};

	// 门连接的下一个房间
	class NextRoom final
	{
		entt::entity next_room;
	};
}
