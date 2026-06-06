// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/resource_fwd.hpp>

#include <blueprint/def.hpp>

#include <component/transform.hpp>
#include <component/render.hpp>
#include <component/tags.hpp>

#include <SFML/Graphics/Color.hpp>

namespace pd::component::particle
{
	// transform --> 粒子变换
	// render --> 粒子渲染
	// tags --> 粒子标签

	// 粒子所有者
	// 所有者一般是粒子发射器,且其可能早于粒子销毁(需检查)
	class Owner final
	{
	public:
		entt::entity owner;
	};

	// 着色器
	class Shader final
	{
	public:
		manager::shader_handler shader;
	};

	// 总存活时间
	class Duration final
	{
	public:
		sf::Time duration;
	};

	// 已存活时间
	class Elapsed final
	{
	public:
		sf::Time elapsed;
	};

	// 着色器参数
	class ShaderParams final
	{
	public:
		sf::Color start_color;
		sf::Color end_color;
		float start_scale;
		float end_scale;
	};
}
