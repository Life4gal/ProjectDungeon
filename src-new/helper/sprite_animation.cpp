// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/sprite_animation.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::helper
{
	using namespace component::sprite_animation;

	auto SpriteAnimation::get_next_frame_index(const FramesCount& frames_count, const Index& index, const Mode& mode, const Direction& direction) noexcept -> index_type
	{
		const auto last_index = frames_count.frames_count - 1;
		const auto current = index.index;

		// 如果到了最后一帧(或第一帧)且不是循环动画则返回animation_ended
		if (const auto end_frame_index = direction == Direction::BACKWARD ? 0 : last_index;
			current == end_frame_index and mode == Mode::ONE_SHOT)
		{
			return animation_ended;
		}

		const auto next_frame_index = direction == Direction::BACKWARD ? (current == 0 ? last_index : current - 1) : (current + 1) % frames_count.frames_count;
		return next_frame_index;
	}

	auto SpriteAnimation::get_next_frame_index(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<FramesCount,Index,Mode, Direction>(entity_with_animation)));

		const auto& frames_count = registry.get<const FramesCount>(entity_with_animation);
		const auto& index = registry.get<const Index>(entity_with_animation);
		const auto& mode = registry.get<const Mode>(entity_with_animation);
		const auto& direction = registry.get<const Direction>(entity_with_animation);

		return get_next_frame_index(frames_count, index, mode, direction);
	}

	auto SpriteAnimation::set_next_frame(const FramesCount& frames_count, Timer& timer, Index& index, const Mode& mode, const Direction& direction) noexcept -> index_type
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

	auto SpriteAnimation::set_next_frame(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		const auto& frames_count = registry.get<const FramesCount>(entity_with_animation);
		auto& timer = registry.get<Timer>(entity_with_animation);
		auto& index = registry.get<Index>(entity_with_animation);
		const auto& mode = registry.get<const Mode>(entity_with_animation);
		const auto& direction = registry.get<const Direction>(entity_with_animation);

		const auto next_frame_index = set_next_frame(frames_count, timer, index, mode, direction);

		// 标记为已结束
		if (next_frame_index == animation_ended)
		{
			end(registry, entity_with_animation);
		}

		return next_frame_index;
	}

	auto SpriteAnimation::jump_to_next_frame(const FramesCount& frames_count, Index& index, const Mode& mode, const Direction& direction) noexcept -> index_type
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

	auto SpriteAnimation::jump_to_next_frame(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		const auto& frames_count = registry.get<const FramesCount>(entity_with_animation);
		auto& index = registry.get<Index>(entity_with_animation);
		const auto& mode = registry.get<const Mode>(entity_with_animation);
		const auto& direction = registry.get<const Direction>(entity_with_animation);

		const auto next_frame_index = jump_to_next_frame(frames_count, index, mode, direction);

		// 标记为已结束
		if (next_frame_index == animation_ended)
		{
			end(registry, entity_with_animation);
		}

		return next_frame_index;
	}

	auto SpriteAnimation::set_frame(index_type frame_index, const FramesCount& frames_count, Timer& timer, Index& index) noexcept -> index_type
	{
		// 跳转到指定帧
		frame_index = jump_to_frame(frame_index, frames_count, index);

		// 设置帧计时器
		timer.elapsed = sf::Time::Zero;

		return frame_index;
	}

	auto SpriteAnimation::set_frame(entt::registry& registry, const entt::entity entity_with_animation, const index_type frame_index) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<FramesCount, Timer, Index>(entity_with_animation)));

		const auto& frames_count = registry.get<const FramesCount>(entity_with_animation);
		auto& timer = registry.get<Timer>(entity_with_animation);
		auto& index = registry.get<Index>(entity_with_animation);

		return set_frame(frame_index, frames_count, timer, index);
	}

	auto SpriteAnimation::jump_to_frame(index_type frame_index, const FramesCount& frames_count, Index& index) noexcept -> index_type
	{
		// 确保帧数不会超出总量
		frame_index %= frames_count.frames_count;

		// 设置当前帧索引
		index.index = frame_index;
		return frame_index;
	}

	auto SpriteAnimation::jump_to_frame(entt::registry& registry, const entt::entity entity_with_animation, const index_type frame_index) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<FramesCount, Index>(entity_with_animation)));

		const auto& frames_count = registry.get<const FramesCount>(entity_with_animation);
		auto& index = registry.get<Index>(entity_with_animation);

		return jump_to_frame(frame_index, frames_count, index);
	}

	auto SpriteAnimation::looping(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.emplace_or_replace<Mode>(entity_with_animation, Mode::LOOP);

		// 如果非循环动画之前已经播放完毕,则需要移除已结束的标记
		registry.remove<Ended>(entity_with_animation);
	}

	auto SpriteAnimation::unlooping(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.emplace_or_replace<Mode>(entity_with_animation, Mode::ONE_SHOT);
	}

	auto SpriteAnimation::is_looping(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<Mode>(entity_with_animation)));

		const auto& mode = registry.get<const Mode>(entity_with_animation);

		return mode == Mode::ONE_SHOT;
	}

	auto SpriteAnimation::reverse(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.emplace_or_replace<Direction>(entity_with_animation, Direction::BACKWARD);
	}

	auto SpriteAnimation::is_reversed(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<Direction>(entity_with_animation)));

		const auto& direction = registry.get<const Direction>(entity_with_animation);

		return direction == Direction::BACKWARD;
	}

	auto SpriteAnimation::pause(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.emplace_or_replace<Paused>(entity_with_animation);
	}

	auto SpriteAnimation::unpause(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.remove<Paused>(entity_with_animation);
	}

	auto SpriteAnimation::is_paused(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return registry.all_of<Paused>(entity_with_animation);
	}

	auto SpriteAnimation::end(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.emplace_or_replace<Ended>(entity_with_animation);
	}

	auto SpriteAnimation::is_ended(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return registry.all_of<Ended>(entity_with_animation);
	}
}
