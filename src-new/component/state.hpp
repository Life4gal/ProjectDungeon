// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::component::state
{
	// 处于相机(可视)区域内
	class InCameraArea {};

	// 实体隐藏(不渲染)
	class Invisible final {};

	// 实体已死亡(可以被销毁)
	class Dead final {};
}
