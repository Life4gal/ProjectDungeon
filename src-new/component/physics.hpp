// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <box2d/id.h>

namespace pd::component::physics
{
	// 物理体ID
	class BodyId final
	{
	public:
		b2BodyId body_id;
	};
}
