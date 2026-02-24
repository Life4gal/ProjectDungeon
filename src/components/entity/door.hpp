// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

#include <config/types.hpp>

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

	// 门的方向
	class Direction final
	{
	public:
		config::DoorDirection direction;
	};

	// 门连接的下一个房间(使用房间ID字符串)
	class TargetRoom final
	{
	public:
		std::string room_id;
	};

	// 门的碰撞体尺寸(物理世界单位,半宽半高)
	// box2d不允许简单地将shape从传感器改为一般碰撞体(反之亦然),只能销毁原有shape并创建新shape
	// 所以我们需要保存门的碰撞体大小,以避免解锁/锁定门时需要再次指定门信息
	class CollisionSize final
	{
	public:
		float half_width;
		float half_height;
	};
}
