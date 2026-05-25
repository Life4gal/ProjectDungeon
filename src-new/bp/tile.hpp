// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <optional>

#include <bp/detail/render.hpp>
#include <bp/detail/collision.hpp>

namespace pd::bp
{
	// 瓦片
	class Tile final
	{
	public:
		// 瓦片的位置
		Position position;

		// 渲染
		Sprite sprite;

		// 碰撞体
		std::optional<Collision> collision;
	};
}
