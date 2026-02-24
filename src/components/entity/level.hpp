// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/dungeon.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::components::level
{
	class CurrentLevel final
	{
	public:
		config::Level level;
	};

	class CurrentTileSet final
	{
	public:
		config::TileSet tileset;
	};

	class CurrentRoom final
	{
	public:
		entt::entity room_entity;
	};

	class RoomOffset final
	{
	public:
		float offset_x;
		float offset_y;
	};

	class RoomCleared final {};
}
