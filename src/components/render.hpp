// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <SFML/Graphics/Color.hpp>

namespace pd::components::render
{
	// transform::Position --> 渲染的位置
	// transform::Scale --> 渲染的缩放
	// transform::Rotation --> 渲染的旋转

	// 当前渲染的纹理
	class Texture final
	{
	public:
		// 当前使用纹理路径
		std::filesystem::path texture_path;

		// 当前纹理位置(如果是一整张图一般为0/0,否则为在图集中的位置)
		int frame_x;
		int frame_y;

		// 当前纹理大小(如果是一整张图一般为纹理大小,否则为在图集中的大小)
		int frame_width;
		int frame_height;
	};

	// 当前渲染的颜色
	class Color final
	{
	public:
		sf::Color color;
	};
} // namespace pd::components::render
