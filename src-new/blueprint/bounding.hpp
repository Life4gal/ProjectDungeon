// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/collision.hpp>

namespace pd::blueprint
{
	class Bounding final
	{
	public:
		using segment_type = CollisionShape::Segment;

		// 边界的位置
		// 绝对位置(刚体位置)
		Position position;

		// 线段位置
		// 相对位置(相对于刚体的偏移)
		// 以房间坐标为(0,0),房间大小为(90,90),房间内瓦片大小为10X10,房间邻居只有右+下为例:
		// position = {0,0}
		// bounding =
		// {
		//  // 左上到右上
		//  { {10,10}, {80,0} },
		//  // 左上到左下
		//  { {10, 10}, {10,80} },
		//  // 右上到右下,中间有间隔(门)(别忘了门区域也需要限制,否则可能从该侧进入墙壁区域)
		//  { {80,10}, {80,40} }, { {80,40}, {90,40} }, { {80,50}, {80,80} },
		//  左下到右下,中间有间隔(门)(别忘了门区域也需要限制,否则可能从该侧进入墙壁区域)
		//  { {10,80}, {40,80} }, { {40,80}, {40,90} }, { {50,80}, {80,80} },
		// }
		std::vector<segment_type> segments;
	};
}
