// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/projectile.hpp>

namespace pd::designer
{
	class Projectile final
	{
	public:
		[[nodiscard]] static auto standard() noexcept -> blueprint::Projectile;
	};
}
