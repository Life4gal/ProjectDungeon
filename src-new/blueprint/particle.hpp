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
		StaticSprite sprite;
		// 着色器路径
		std::string shader;
		// 存在时间
		LimitedLife::Time lifetime;

		// TODO: 着色器参数如何配置?
		std::uint32_t start_color;
		std::uint32_t end_color;
		float start_scale;
		float end_scale;
	};
}
