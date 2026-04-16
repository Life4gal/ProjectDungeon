// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::components::transform
{
	// 像素位置
	// 如果实体存在物理刚体组件则该组件的位置会随物理刚体的位置自动更新,否则该组件的位置由Transform::set_position接口设置
	class Position
	{
	public:
		sf::Vector2f position;
	};

	// 缩放
	class Scale
	{
	public:
		sf::Vector2f scale;
	};

	// 旋转
	// 如果实体存在物理刚体组件则该组件的旋转会随物理刚体的旋转自动更新,否则该组件的旋转由Transform::set_rotation接口设置
	class Rotation
	{
	public:
		sf::Angle rotation;
	};
}
