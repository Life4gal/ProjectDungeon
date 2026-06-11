// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

#include <component/render.hpp>

namespace pd::component::animation
{
	// render --> 角色渲染

	// 当前朝向
	using blueprint::Direction;

	// 索引
	class Groups
	{
	public:
		// blueprint::Animation::size_type
		using size_type = std::uint32_t;

		// blueprint::Animation::Group
		class Group final
		{
		public:
			size_type begin_frame;
			size_type end_frame;
		};

		// blueprint::Animation::groups_type
		using groups_type = std::array<Group, 4>;

		groups_type groups;
	};
}
