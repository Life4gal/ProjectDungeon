// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/resource_fwd.hpp>

#include <SFML/System/Vector2.hpp>
// #include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>

namespace pd::component::sprite
{
	// ====================
	// 纹理
	// ====================

	// 纹理的资源句柄
	class Texture final
	{
	public:
		manager::texture_handler texture;
	};

	// 纹理位置
	// 如果使用整张纹理渲染则为{0,0}
	class TexturePosition final
	{
	public:
		sf::Vector2f position;
	};

	// 纹理的大小
	// 如果使用整张纹理渲染则为整张纹理的大小
	class TextureSize final
	{
	public:
		sf::Vector2f size;
	};

	// 纹理的原点(一般是TextureSize/2)
	class TextureOrigin final
	{
	public:
		sf::Vector2f origin;
	};

	// ====================
	// 渲染
	// ====================

	// 渲染的位置的偏移
	// 实际渲染位置 = position::World + offset
	// 该组件允许我们*渲染*的位置与*实际*的位置不同
	// 我们可以基于此做到一些动画效果(而不影响实体实际的位置)
	class RenderPositionOffset final
	{
	public:
		sf::Vector2f offset;
	};

	// 渲染的缩放
	// 该组件允许我们*渲染*的大小与*实际*的大小不同
	// 我们可以基于此做到一些动画效果(而不影响实体的实际大小)
	class RenderScale final
	{
	public:
		sf::Vector2f scale;
	};

	// 渲染的旋转
	class RenderRotation final
	{
	public:
		sf::Angle rotation;
	};

	// 渲染的颜色
	// 默认为白色
	class RenderColor final
	{
	public:
		sf::Color color;
	};
}
