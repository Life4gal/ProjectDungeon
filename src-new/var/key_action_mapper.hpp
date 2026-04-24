// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <menu/menu.hpp>

namespace pd::var
{
	inline auto key_action_mapper = std::make_unique<menu::ActionMapper>(); // NOLINT(bugprone-throwing-static-initialization)
}
