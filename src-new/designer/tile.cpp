// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/tile.hpp>

#include <manager/random.hpp>

#include <designer/room.hpp>

namespace pd::designer
{
	auto Tile::floor() noexcept -> blueprint::Tile
	{
		constexpr blueprint::Size size{.width = Room::tile_width, .height = Room::tile_height};
		constexpr blueprint::Offset pivot{.x = size.width / 2, .y = size.height / 2};

		return
		{
				.position = {.x = 0, .y = 0},
				.sprite =
				{
						.sprite = blueprint::Sprite::Static
						{
								.texture = "./assets/wall+floor.png",
								.uv_position = {.x = size.width * 9, .y = size.height * 4},
								.uv_size = size,
								.pivot = pivot,
								.render_layer = blueprint::RenderLayer::FLOOR,
						},
				},
				.collision = std::nullopt,
		};
	}

	auto Tile::wall() noexcept -> blueprint::Tile
	{
		constexpr blueprint::Size size{.width = Room::tile_width, .height = Room::tile_height};
		constexpr blueprint::Offset pivot{.x = size.width / 2, .y = size.height / 2};

		return
		{
				.position = {.x = 0, .y = 0},
				.sprite =
				{
						.sprite = blueprint::Sprite::Static
						{
								.texture = "./assets/wall+floor.png",
								// 随机样式
								.uv_position = {.x = static_cast<float>(manager::Random::int_inclusive(0, 8)) * size.width, .y = size.height},
								.uv_size = size,
								.pivot = pivot,
								.render_layer = blueprint::RenderLayer::WALL,
						},
				},
				.collision =
				// 墙壁的阻挡作用已被房间边界取代
				// 除非是位于房间内的墙壁?
				std::nullopt,
				// blueprint::Collision
				// {
				// 		.def =
				// 		{
				// 				.type = blueprint::CollisionBodyType::STATIC,
				// 				.fixed_rotation = true,
				// 				.is_bullet = false,
				// 		},
				// 		.shapes =
				// 		{
				// 				// 矩形碰撞体
				// 				{
				// 						.def =
				// 						{
				// 								.material = {.friction = 0.6f, .restitution = 0},
				// 								.density = 0,
				// 								.category = blueprint::CollisionCategory::WALL,
				// 								.mask = blueprint::CollisionMask::WALL,
				// 								.is_sensor = false,
				// 								.enable_sensor_events = false,
				// 								.enable_contact_events = false,
				// 						},
				// 						.shape =
				// 						blueprint::CollisionShape::Box
				// 						{
				// 								.size = {.width = tile_width, .height = tile_height},
				// 						},
				// 				},
				// 				//
				// 		},
				// },
		};
	}
}
