// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <scene/category.hpp>

namespace pd::event::scene
{
	class Switched final
	{
	public:
		pd::scene::Category category;
	};
}
