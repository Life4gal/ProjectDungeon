// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/dynamic_sprite.hpp>

#include <prometheus/platform/os.hpp>

namespace pd::helper
{
	using namespace component;
	using namespace render::dynamic_sprite;

	auto DynamicSprite::get_next_frame_index(
		const BeginFrameIndex& begin_frame,
		const EndFrameIndex& end_frame,
		const CurrentFrameIndex& index,
		const AnimationMode& mode,
		const AnimationDirection& direction
	) noexcept -> index_type
	{
		// 如果是向前播放
		if (direction == AnimationDirection::FORWARD)
		{
			// 如果当前索引等于最后一帧索引
			if (index.index == end_frame.index)
			{
				// 如果单次播放
				if (mode == AnimationMode::ONE_SHOT)
				{
					// 结束
					return animation_ended;
				}

				// 下一帧为第一帧
				return begin_frame.index;
			}

			// 下一帧为为下一帧
			return index.index + 1;
		}

		// 如果是反向播放
		if (direction == AnimationDirection::BACKWARD)
		{
			// 如果当前索引等于第一帧索引
			if (index.index == begin_frame.index)
			{
				// 如果单次播放
				if (mode == AnimationMode::ONE_SHOT)
				{
					// 结束
					return animation_ended;
				}

				// 下一帧为最后一帧
				return end_frame.index;
			}

			// 下一帧为上一帧
			return index.index - 1;
		}

		PROMETHEUS_PLATFORM_UNREACHABLE();
	}

	auto DynamicSprite::set_next_frame(
		const BeginFrameIndex& begin_frame,
		const EndFrameIndex& end_frame,
		CurrentFrameIndex& index,
		CurrentFrameElapsed& elapsed,
		const AnimationMode& mode,
		const AnimationDirection& direction
	) noexcept -> index_type
	{
		// 获取下一帧索引
		const auto next_frame_index = get_next_frame_index(begin_frame, end_frame, index, mode, direction);

		// 如果动画已结束则什么也不做
		if (next_frame_index == animation_ended)
		{
			return animation_ended;
		}

		// 设置为指定帧
		return set_frame(next_frame_index, begin_frame, end_frame, index, elapsed);
	}

	auto DynamicSprite::jump_to_next_frame(
		const BeginFrameIndex& begin_frame,
		const EndFrameIndex& end_frame,
		CurrentFrameIndex& index,
		const AnimationMode& mode,
		const AnimationDirection& direction
	) noexcept -> index_type
	{
		// 获取下一帧索引
		const auto next_frame_index = get_next_frame_index(begin_frame, end_frame, index, mode, direction);

		// 如果动画已结束则什么也不做
		if (next_frame_index == animation_ended)
		{
			return animation_ended;
		}

		// 跳转到指定帧
		return jump_to_frame(next_frame_index, begin_frame, end_frame, index);
	}

	auto DynamicSprite::set_frame(
		index_type frame_index,
		const BeginFrameIndex& begin_frame,
		const EndFrameIndex& end_frame,
		CurrentFrameIndex& index,
		CurrentFrameElapsed& elapsed
	) noexcept -> index_type
	{
		// 跳转到指定帧
		frame_index = jump_to_frame(frame_index, begin_frame, end_frame, index);

		// 设置帧计时器
		elapsed.elapsed = sf::Time::Zero;

		return frame_index;
	}

	auto DynamicSprite::jump_to_frame(
		index_type frame_index,
		const BeginFrameIndex& begin_frame,
		const EndFrameIndex& end_frame,
		CurrentFrameIndex& index
	) noexcept -> index_type
	{
		// 确保帧数不会超出总量
		if (frame_index > end_frame.index)
		{
			const auto count = end_frame.index - begin_frame.index;
			const auto n = frame_index % count;

			frame_index = begin_frame.index + n;
		}

		// 设置当前帧索引
		index.index = frame_index;
		return frame_index;
	}
}
