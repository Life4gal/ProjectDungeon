// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::blueprint
{
	class Position final
	{
	public:
		float x;
		float y;
	};

	class Scale final
	{
	public:
		float x;
		float y;
	};

	class Rotation final
	{
	public:
		// degree
		float rotation;
	};

	// 暂时没有合适的地方,先放这吧 :)
	class Size final
	{
	public:
		float width;
		float height;
	};
}
