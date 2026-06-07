// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::blueprint
{
	// 世界坐标
	class Position final
	{
	public:
		float x;
		float y;
	};

	// 缩放
	class Scale final
	{
	public:
		float x;
		float y;
	};

	// 旋转
	class Rotation final
	{
	public:
		// degree
		float rotation;
	};
}
