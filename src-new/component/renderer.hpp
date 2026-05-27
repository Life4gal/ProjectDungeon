// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::component::renderer
{
	// 标记渲染视图需要进行排序
	// 这会在创建/改动RenderLayer组件时设置
	// factory/render.cpp
	// update/render.cpp
	class SortRequired final {};
}
