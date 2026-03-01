// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::config
{
	// 门的所在方向
	enum class DoorDirection : std::uint8_t
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

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
} // namespace pd::config
