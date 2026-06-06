// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/particle_emitter.hpp>

namespace pd::designer
{
	class ParticleEmitter final
	{
	public:
		// 飞弹拖尾效果
		[[nodiscard]] static auto projectile_trail() noexcept -> blueprint::ParticleEmitter;
	};
}
