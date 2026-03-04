// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility>
#include <string_view>

namespace pd::config
{
	// 门的所在方向
	enum class DoorDirection : std::uint8_t
	{
		// 00
		UP = 0b00,
		// 01
		DOWN = 0b01,
		// 11
		LEFT = 0b11,
		// 10
		RIGHT = 0b10,
	};

	// 门的反方向
	// 一般用于确定下一个房间对应门的方向
	[[nodiscard]] constexpr auto operator-(const DoorDirection direction) noexcept -> DoorDirection
	{
		return static_cast<DoorDirection>(std::to_underlying(direction) ^ 0b01);
	}

	// 门索引
	// 用于访问一个array<T, 4>(或者类似的)容器
	[[nodiscard]] constexpr auto index_of(const DoorDirection direction) noexcept -> std::size_t
	{
		return std::to_underlying(direction);
	}

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

	[[nodiscard]] constexpr auto room_type_of(const std::string_view type) noexcept -> RoomType
	{
		if (type == "STARTING")
		{
			return RoomType::STARTING;
		}
		if (type == "NORMAL")
		{
			return RoomType::NORMAL;
		}
		if (type == "TREASURE")
		{
			return RoomType::TREASURE;
		}
		if (type == "KEY")
		{
			return RoomType::KEY;
		}
		if (type == "BOSS")
		{
			return RoomType::BOSS;
		}
		if (type == "EXIT")
		{
			return RoomType::EXIT;
		}

		return RoomType::NORMAL;
	}
} // namespace pd::config
