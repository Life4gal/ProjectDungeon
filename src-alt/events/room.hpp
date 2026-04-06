// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>
#include <functional>

namespace pd::components::floor
{
	class RoomInfo;
	class RoomEntity;
}

namespace pd::events::room
{
	// 激活(进入)房间
	class Active final
	{
	public:
		std::reference_wrapper<const components::floor::RoomInfo> info;
		std::reference_wrapper<const components::floor::RoomEntity> entity;
	};

	// 房间被清理
	class Cleared final
	{
	public:
		std::reference_wrapper<const components::floor::RoomInfo> info;
		std::reference_wrapper<const components::floor::RoomEntity> entity;
	};

	// 禁用(离开)房间
	class Inactive final
	{
	public:
		std::reference_wrapper<const components::floor::RoomInfo> info;
		std::reference_wrapper<const components::floor::RoomEntity> entity;
	};
}
