// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/asset_fwd.hpp>

#include <SFML/System/Vector2.hpp>
// #include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>

namespace pd::components::sprite
{
	// 渲染纹理
	class Texture final
	{
	public:
		manager::AssetId id;
	};

	// 纹理位置
	// 如果使用整张纹理渲染则为{0,0}
	class Position final
	{
	public:
		sf::Vector2i position;
	};

	// 纹理大小
	// 如果使用整张纹理渲染则为整张纹理的大小
	class Size final
	{
	public:
		sf::Vector2i size;
	};

	// 渲染区域原点(一般是size/2)
	class Origin final
	{
	public:
		sf::Vector2i origin;
	};

	// 颜色
	// 一般是白色,可以改变颜色以做到像是受击变色之类的效果
	class Color final
	{
	public:
		sf::Color color;
	};

	// 缩放
	// 一般是{1,1},可以改变缩放以做到像是受击放大/缩小之类的效果
	class Scale final
	{
	public:
		sf::Vector2f scale;
	};

	// 标记实体隐藏(不渲染)
	class Invisible final {};
}
