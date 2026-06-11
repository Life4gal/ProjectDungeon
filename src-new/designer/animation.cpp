// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/animation.hpp>

namespace pd::designer
{
	auto Animation::rat() noexcept -> blueprint::Animation
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 下 60*136
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 30, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 286, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 540, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 800, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 上 60*136
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 30, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 286, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 540, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 800, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 左 140*116
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 6, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 262, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 520, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 774, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 右 140*116
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 44, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 282, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 556, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 808, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
				},
				.render_layer = blueprint::RenderLayer::ENEMY,
				.mode = blueprint::AnimationMode::LOOP,
				.direction = blueprint::AnimationDirection::FORWARD,
				.pause = false,
		};
		// 各个方向的动画索引
		blueprint::Animation::groups_type groups{};
		groups[std::to_underlying(blueprint::Direction::SOUTH)] = {.begin_frame = 0, .end_frame = 3};
		groups[std::to_underlying(blueprint::Direction::NORTH)] = {.begin_frame = 4, .end_frame = 7};
		groups[std::to_underlying(blueprint::Direction::WEST)] = {.begin_frame = 8, .end_frame = 11};
		groups[std::to_underlying(blueprint::Direction::EAST)] = {.begin_frame = 12, .end_frame = 15};

		return {.sprite_sheet = std::move(sprite), .groups = groups};
	}

	auto Animation::slime() noexcept -> blueprint::Animation
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 下 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 42}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 42}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 42}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 46}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 上 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 左 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 右 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
				},
				.render_layer = blueprint::RenderLayer::ENEMY,
				.mode = blueprint::AnimationMode::LOOP,
				.direction = blueprint::AnimationDirection::FORWARD,
				.pause = false,
		};
		// 各个方向的动画索引
		blueprint::Animation::groups_type groups{};
		groups[std::to_underlying(blueprint::Direction::SOUTH)] = {.begin_frame = 0, .end_frame = 3};
		groups[std::to_underlying(blueprint::Direction::NORTH)] = {.begin_frame = 4, .end_frame = 7};
		groups[std::to_underlying(blueprint::Direction::WEST)] = {.begin_frame = 8, .end_frame = 11};
		groups[std::to_underlying(blueprint::Direction::EAST)] = {.begin_frame = 12, .end_frame = 15};

		return {.sprite_sheet = std::move(sprite), .groups = groups};
	}

	auto Animation::bat() noexcept -> blueprint::Animation
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 下 100*100
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 12, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 268, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 524, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 780, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 上 120*100
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 2, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 256, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 510, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 770, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 左 150*100
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 0, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 242, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 500, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 754, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 右 136*110
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 0, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 250, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 508, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 760, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
				},
				.render_layer = blueprint::RenderLayer::ENEMY,
				.mode = blueprint::AnimationMode::LOOP,
				.direction = blueprint::AnimationDirection::FORWARD,
				.pause = false,
		};
		// 各个方向的动画索引
		blueprint::Animation::groups_type groups{};
		groups[std::to_underlying(blueprint::Direction::SOUTH)] = {.begin_frame = 0, .end_frame = 3};
		groups[std::to_underlying(blueprint::Direction::NORTH)] = {.begin_frame = 4, .end_frame = 7};
		groups[std::to_underlying(blueprint::Direction::WEST)] = {.begin_frame = 8, .end_frame = 11};
		groups[std::to_underlying(blueprint::Direction::EAST)] = {.begin_frame = 12, .end_frame = 15};

		return {.sprite_sheet = std::move(sprite), .groups = groups};
	}

	auto Animation::player_default() noexcept -> blueprint::Animation
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 0, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
						// 第二帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 64, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
						// 第三帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 128, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
						// 第四帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 192, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
				},
				.render_layer = blueprint::RenderLayer::PLAYER,
				.mode = blueprint::AnimationMode::LOOP,
				.direction = blueprint::AnimationDirection::FORWARD,
				.pause = false,
		};

		// 各个方向的动画索引
		blueprint::Animation::groups_type groups{};
		groups[std::to_underlying(blueprint::Direction::SOUTH)] = {.begin_frame = 0, .end_frame = 3};
		groups[std::to_underlying(blueprint::Direction::NORTH)] = {.begin_frame = 0, .end_frame = 3};
		groups[std::to_underlying(blueprint::Direction::WEST)] = {.begin_frame = 0, .end_frame = 3};
		groups[std::to_underlying(blueprint::Direction::EAST)] = {.begin_frame = 0, .end_frame = 3};

		return {.sprite_sheet = std::move(sprite), .groups = groups};
	}
}
