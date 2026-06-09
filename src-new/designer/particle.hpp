// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/particle.hpp>

namespace pd::designer
{
	class Particle final
	{
	public:
		// 飞弹
		[[nodiscard]] static auto projectile() noexcept -> blueprint::Particle;
	};
}
