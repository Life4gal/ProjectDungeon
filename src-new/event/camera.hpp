// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::event::camera
{
	class Set final
	{
	public:
		float x;
		float y;
		float width;
		float height;
	};

	class Move final
	{
	public:
		float x;
		float y;
	};

	class Resize final
	{
	public:
		float width;
		float height;
	};
}
