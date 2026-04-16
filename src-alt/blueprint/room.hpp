// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/def.hpp>

#include <game/constants.hpp>

#include <utility/fixed_matrix.hpp>

namespace pd::blueprint
{
	class Tile final
	{
	public:
		TileCollision collision_logical;
		TileTransform transform;
		TileOverlayObject overlay_object;

		//
	};

	class Room final
	{
	public:
		using layout_type = utility::FixedMatrix<Tile, game::RoomHorizontalGrid, game::RoomVerticalGrid>;
		using size_type = layout_type::size_type;

		[[nodiscard]] static auto generate_standard() -> layout_type;
	};
}
