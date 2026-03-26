// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Time.hpp>

namespace pd::components::door
{
	// ==========================================
	// 非实体组件
	// ==========================================

	enum class Direction : std::uint8_t
	{
		NORTH = 0b00,
		SOUTH = 0b01,
		WEST = 0b10,
		EAST = 0b11,

		COUNT
	};

	// [[nodiscard]] constexpr auto operator-(const Direction direction) noexcept -> Direction
	// {
	// 	return static_cast<Direction>(std::to_underlying(direction) ^ 0b01);
	// }

	enum class Type : std::uint8_t
	{
		// 标准门
		STANDARD,
		// BOSS门
		BOSS,
		// 出口门
		EXIT,
	};

	// ==========================================
	// 上下文组件
	// ==========================================

	//

	// ==========================================
	// 实体组件
	// ==========================================

	// 上面的组件我们不附加在实体上,而是保存在房间的组件中

	class Locked {};

	enum class State : std::uint8_t
	{
		// 已关闭
		CLOSED,
		// 正在关闭
		CLOSING,
		// 已开启
		OPENED,
		// 正在开启
		OPENING,
	};

	// 开关门动画计时器
	// 仅在播放动画时存在
	class Timer final
	{
	public:
		sf::Time timer;
	};
}
