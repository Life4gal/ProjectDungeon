// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::config
{
	// 房间类型
	enum class RoomType : std::uint8_t
	{
		// 初始房间 - 无敌人
		STARTING,
		// 普通房间 - 包含敌人/道具/机关
		NORMAL,
		// 宝藏房间(奖励房间) - 无敌人,纯奖励
		TREASURE,
		// 钥匙房间 - 生成BOSS房钥匙
		KEY,
		// Boss房间 - 击败BOSS才能继续
		BOSS,
		// 出口房间 - 进入下一关卡
		EXIT,
	};

	// 房间内门的所在方向
	enum class DoorDirection : std::uint8_t
	{
		// 上方的门
		UP,
		// 下方的门
		DOWN,
		// 左侧的门
		LEFT,
		// 右侧的门
		RIGHT,
	};

	// 敌人类型
	enum class EnemyType : std::uint8_t
	{
		// 普通敌人
		NORMAL = 0,
		// Boss敌人
		BOSS = 1,
		// 奖励敌人
		REWARD = 2,
	};

	// 移动类型
	enum class Movement : std::uint8_t
	{
		// 地面移动
		GROUND = 0b0000'0001,
		// 空中移动
		AERIAL = 0b0000'0010,
		// 复合移动(地面+空中)
		MISC = GROUND | AERIAL,
	};
}
