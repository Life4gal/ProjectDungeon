// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/floor.hpp>

#include <designer/room.hpp>

namespace pd::designer
{
	auto Floor::standard(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Floor
	{
		const blueprint::Position position
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
		};
		blueprint::Sprite sprite
		{
				.texture = "./assets/tileset/floor.png",
				.position = {.x = 0, .y = 0},
				.size = {.width = Room::tile_width, .height = Room::tile_height},
				.origin = {.x = Room::tile_origin_x, .y = Room::tile_origin_y},
				.scale = {.x = 1, .y = 1},
		};

		return {.position = position, .sprite = std::move(sprite)};
	}
}
