// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

#include <blueprint/detail/position.hpp>
#include <blueprint/detail/size.hpp>
#include <blueprint/detail/scale.hpp>

namespace pd::blueprint
{
	class Sprite final
	{
	public:
		// 纹理路径
		std::string texture;
		// 纹理坐标
		Position position;
		// 纹理大小
		Size size;
		// 纹理原点
		Position origin;
		// 纹理缩放
		Scale scale;
	};
}
