// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/door.hpp>

#include <designer/room.hpp>

namespace pd::designer
{
	namespace
	{
		class Sensor final
		{
		public:
			blueprint::Offset offset;
			blueprint::Size size;
		};

		[[nodiscard]] constexpr auto sensor_of(const blueprint::Direction direction, const blueprint::Size size) noexcept -> Sensor
		{
			// 感应区域占比
			constexpr auto sensor_area_ratio = 0.2f;

			// 感应区域大小
			const auto sensor_area_width = size.width * sensor_area_ratio;
			const auto sensor_area_height = size.height * sensor_area_ratio;

			// 北面
			if (direction == blueprint::Direction::NORTH)
			{
				constexpr auto sensor_x = 0.0f;
				const auto sensor_y = -static_cast<float>(size.height) / 2 + sensor_area_height / 2;

				return {.offset = {.x = sensor_x, .y = sensor_y}, .size = {.width = size.width, .height = sensor_area_height}};
			}

			// 南面
			if (direction == blueprint::Direction::SOUTH)
			{
				constexpr auto sensor_x = 0.0f;
				const auto sensor_y = static_cast<float>(size.height) / 2 - sensor_area_height / 2;

				return {.offset = {.x = sensor_x, .y = sensor_y}, .size = {.width = size.width, .height = sensor_area_height}};
			}

			// 西面
			if (direction == blueprint::Direction::WEST)
			{
				const auto sensor_x = -static_cast<float>(size.width) / 2 + sensor_area_width / 2;
				constexpr auto sensor_y = 0.0f;

				return {.offset = {.x = sensor_x, .y = sensor_y}, .size = {.width = sensor_area_width, .height = size.width}};
			}

			// 东面
			if (direction == blueprint::Direction::EAST)
			{
				const auto sensor_x = static_cast<float>(size.width) / 2 - sensor_area_width / 2;
				constexpr auto sensor_y = 0.0f;

				return {.offset = {.x = sensor_x, .y = sensor_y}, .size = {.width = sensor_area_width, .height = size.width}};
			}

			std::unreachable();
		}
	}

	auto Door::standard(const blueprint::Direction direction) noexcept -> blueprint::Door
	{
		constexpr blueprint::Size size{.width = Room::tile_width, .height = Room::tile_height};
		constexpr blueprint::Offset pivot{.x = size.width / 2, .y = size.height / 2};

		const auto [sensor_offset, sensor_size] = sensor_of(direction, size);

		return
		{
				.position = {.x = 0, .y = 0},
				.direction = direction,
				.door_offset = {.x = 0, .y = 0},
				.door_size = size,
				.sensor_offset = sensor_offset,
				.sensor_size = sensor_size,
				.sprite =
				{
						.frames =
						{
								// 第一帧
								{.texture = "./assets/tileset/door.png", .uv_position = {.x = size.width * 0, .y = 0}, .uv_size = size, .pivot = pivot, .duration_ms = 100},
								// 第二帧
								{.texture = "./assets/tileset/door.png", .uv_position = {.x = size.width * 1, .y = 0}, .uv_size = size, .pivot = pivot, .duration_ms = 100},
								// 第三帧
								{.texture = "./assets/tileset/door.png", .uv_position = {.x = size.width * 2, .y = 0}, .uv_size = size, .pivot = pivot, .duration_ms = 100},
								// 第四帧
								{.texture = "./assets/tileset/door.png", .uv_position = {.x = size.width * 3, .y = 0}, .uv_size = size, .pivot = pivot, .duration_ms = 100},
						},
						.render_layer = blueprint::RenderLayer::DOOR,
						.looping = false,
						.reversed = false,
						.pause = true,
				},
		};
	}
}
