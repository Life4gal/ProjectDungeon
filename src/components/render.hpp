// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <functional>

#include <config/render_layer.hpp>

#include <SFML/Graphics/Color.hpp>

namespace pd::config
{
	class AnimationFrame;
}

namespace pd::components::render
{
	// ==========================================
	// 上下文组件
	// ==========================================

	//

	// ==========================================
	// 实体组件
	// ==========================================

	// 对config::AnimationFrame的引用
	class AnimationFrame final
	{
	public:
		std::reference_wrapper<const config::AnimationFrame> animation_frame;
	};

	// transform::Position --> 渲染的位置
	// transform::Scale --> 渲染的缩放
	// transform::Rotation --> 渲染的旋转

	// 渲染层级
	class RenderLayer final
	{
	public:
		config::RenderLayer render_layer;
	};

	// 颜色
	class Color final
	{
	public:
		sf::Color color;
	};

	// 标记实体是否应该渲染
	// 仅有包含该标记的实体才会渲染
	class Visible final {};
} // namespace pd::components::render
