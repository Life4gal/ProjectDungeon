// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::component
{
	// ======================================
	// 身份
	// ======================================

	namespace tags
	{
		// 计划任务
		class ScheduledTask final {};

		// 粒子
		class Particle final {};

		// 飞弹
		class Projectile final {};

		// 敌人
		class Enemy final {};

		// NPC
		class Npc final {};

		// 玩家
		class Player final {};

		// 地下城-- 关卡 -- 房间 -- 门
		class Door final {};

		// 地下城-- 关卡 -- 房间 -- 房间边界
		class Bounding final {};

		// 地下城-- 关卡 -- 房间 -- 瓦片
		class Tile final {};

		// 地下城-- 关卡 -- 房间
		class Room final {};
	}

	// ======================================
	// 状态
	// ======================================

	namespace state
	{
		// 实体已死亡
		// 任何含有该标记的实体将在该帧update的末尾(或者说下一帧update的开始)被销毁
		class EntityDead final {};

		// 实体刚刚(上一帧)被创建
		// 该标记理论上只存在一帧
		class EntityNew final {};

		// 实体处于相机(可视)区域内
		// 只有含义该标记的实体才会被更新和渲染
		class InCameraArea final {};

		// 实体禁用渲染
		class DisableRender final {};

		// 实体禁用AI
		class DisableAi final {};
	}
}
