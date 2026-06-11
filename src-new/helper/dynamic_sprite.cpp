// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/dynamic_sprite.hpp>

namespace pd::helper
{
	using namespace component;
	using namespace render::dynamic_sprite;

	auto DynamicSprite::get_next_frame_index(const FramesCount& frames_count, const Index& index, const AnimationMode& mode, const AnimationDirection& direction) noexcept -> index_type
	{
		const auto last_index = frames_count.frames_count - 1;
		const auto current = index.index;

		// 如果到了最后一帧(或第一帧)且不是循环动画则返回animation_ended
		if (const auto end_frame_index = direction == AnimationDirection::BACKWARD ? 0 : last_index;
			current == end_frame_index and mode == AnimationMode::ONE_SHOT)
		{
			return animation_ended;
		}

		const auto next_frame_index = direction == AnimationDirection::BACKWARD ? (current == 0 ? last_index : current - 1) : (current + 1) % frames_count.frames_count;
		return next_frame_index;
	}

	auto DynamicSprite::set_next_frame(const FramesCount& frames_count, Timer& timer, Index& index, const AnimationMode& mode, const AnimationDirection& direction) noexcept -> index_type
	{
		// 获取下一帧索引
		const auto next_frame_index = get_next_frame_index(frames_count, index, mode, direction);

		// 如果动画已结束则什么也不做
		if (next_frame_index == animation_ended)
		{
			return animation_ended;
		}

		// 设置为指定帧
		return set_frame(next_frame_index, frames_count, timer, index);
	}

	auto DynamicSprite::jump_to_next_frame(const FramesCount& frames_count, Index& index, const AnimationMode& mode, const AnimationDirection& direction) noexcept -> index_type
	{
		// 获取下一帧索引
		const auto next_frame_index = get_next_frame_index(frames_count, index, mode, direction);

		// 如果动画已结束则什么也不做
		if (next_frame_index == animation_ended)
		{
			return animation_ended;
		}

		// 跳转到指定帧
		return jump_to_frame(next_frame_index, frames_count, index);
	}

	auto DynamicSprite::set_frame(index_type frame_index, const FramesCount& frames_count, Timer& timer, Index& index) noexcept -> index_type
	{
		// 跳转到指定帧
		frame_index = jump_to_frame(frame_index, frames_count, index);

		// 设置帧计时器
		timer.elapsed = sf::Time::Zero;

		return frame_index;
	}

	auto DynamicSprite::jump_to_frame(index_type frame_index, const FramesCount& frames_count, Index& index) noexcept -> index_type
	{
		// 确保帧数不会超出总量
		frame_index %= frames_count.frames_count;

		// 设置当前帧索引
		index.index = frame_index;
		return frame_index;
	}
}
