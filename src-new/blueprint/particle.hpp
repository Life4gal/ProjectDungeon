// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/limited_life.hpp>
#include <blueprint/detail/render.hpp>

namespace pd::blueprint
{
	// 粒子
	class Particle final
	{
	public:
		// 纹理
		Sprite::Static sprite;

		// 存在时间
		LimitedLife::Time lifetime;
		// render_effect::color::AlphaFade
		float start_alpha;
		float end_alpha;
		// render_effect::scale::Shrink
		float start_scale;
		float end_scale;
	};
}
