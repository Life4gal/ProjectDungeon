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
	class Animation;
}

namespace pd::components::render
{
	// ==========================================
	// 上下文组件
	// ==========================================

	// 标记渲染视图需要进行排序
	// 这会在创建/改动RenderLayer组件时设置
	class SortRequired final {};

	// ==========================================
	// 实体组件
	// ==========================================

	// 对config::Animation的引用
	// 我们将纹理大小/原点等信息放在config::Animation中而不是config::AnimationFrame中
	// 所以我们还需要额外引用该信息
	// 与components::animation::Animation冗余,不过我们不像让其耦合
	class Animation final
	{
	public:
		std::reference_wrapper<const config::Animation> animation;
	};

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

	// 标记实体隐藏(不渲染)
	class Invisible final {};
} // namespace pd::components::render
