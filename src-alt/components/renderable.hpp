// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/asset_fwd.hpp>

#include <SFML/Graphics/Color.hpp>

namespace pd::components::renderable
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

	// 渲染层级
	// 数值小的先渲染,换句话说数值大的将会覆盖数值小的(如果有重叠)
	enum class RenderLayer : std::uint32_t
	{
		//
	};

	// 渲染纹理
	class Texture final
	{
	public:
		manager::AssetId id;
	};

	// 渲染区域
	// 如果使用整张纹理渲染则等于该纹理大小
	class Rect final
	{
	public:
		int x;
		int y;
		int width;
		int height;
	};

	// 渲染区域原点(一般是中心点?)
	class Origin final
	{
	public:
		int x;
		int y;
	};

	// 颜色
	// 一般是白色,可以改变颜色以做到像是受击/闪烁之类的效果
	class Color final
	{
	public:
		sf::Color color;
	};;

	// 标记实体隐藏(不渲染)
	class Invisible final {};
}
