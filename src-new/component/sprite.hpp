// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/resource_fwd.hpp>

#include <component/sprite_effect.hpp>

namespace pd::component::sprite
{
	// sprite_effect --> 精灵特效

	// 纹理的资源句柄
	class Texture final
	{
	public:
		manager::texture_handler texture;
	};

	// 纹理位置
	// 如果使用整张纹理渲染则为{0,0}
	class Position final
	{
	public:
		sf::Vector2f position;
	};

	// 纹理的大小
	// 如果使用整张纹理渲染则为整张纹理的大小
	class Size final
	{
	public:
		sf::Vector2f size;
	};

	// 纹理的原点(一般是TextureSize/2)
	class Origin final
	{
	public:
		sf::Vector2f origin;
	};
}
