// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <accessor/dynamic_sprite.hpp>

#include <component/render.hpp>

#include <helper/dynamic_sprite.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::accessor
{
	using namespace component;
	using namespace render::dynamic_sprite;

	auto DynamicSprite::get_frame_count(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<Frames>(entity_with_animation)));

		const auto& [frames] = registry.get<const Frames>(entity_with_animation);

		return frames.size();
	}

	auto DynamicSprite::get_present_frame_count(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<BeginFrameIndex, EndFrameIndex>(entity_with_animation)));

		const auto& [begin_frame_index] = registry.get<const BeginFrameIndex>(entity_with_animation);
		const auto& [end_frame_index] = registry.get<const EndFrameIndex>(entity_with_animation);

		return end_frame_index - begin_frame_index;
	}

	auto DynamicSprite::get_next_frame_index(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<BeginFrameIndex, EndFrameIndex, CurrentFrameIndex, AnimationMode, AnimationDirection>(entity_with_animation)));

		const auto& begin_frame_index = registry.get<const BeginFrameIndex>(entity_with_animation);
		const auto& end_frame_index = registry.get<const EndFrameIndex>(entity_with_animation);
		auto& current_frame_index = registry.get<CurrentFrameIndex>(entity_with_animation);
		const auto& mode = registry.get<const AnimationMode>(entity_with_animation);
		const auto& direction = registry.get<const AnimationDirection>(entity_with_animation);

		return helper::DynamicSprite::get_next_frame_index(begin_frame_index, end_frame_index, current_frame_index, mode, direction);
	}

	auto DynamicSprite::set_next_frame(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<BeginFrameIndex, EndFrameIndex, CurrentFrameIndex, CurrentFrameElapsed, AnimationMode, AnimationDirection>(entity_with_animation)));

		const auto& begin_frame_index = registry.get<const BeginFrameIndex>(entity_with_animation);
		const auto& end_frame_index = registry.get<const EndFrameIndex>(entity_with_animation);
		auto& current_frame_index = registry.get<CurrentFrameIndex>(entity_with_animation);
		auto& current_frame_elapsed = registry.get<CurrentFrameElapsed>(entity_with_animation);
		const auto& mode = registry.get<const AnimationMode>(entity_with_animation);
		const auto& direction = registry.get<const AnimationDirection>(entity_with_animation);

		const auto next_frame_index = helper::DynamicSprite::set_next_frame(begin_frame_index, end_frame_index, current_frame_index, current_frame_elapsed, mode, direction);

		// 标记为已结束
		if (next_frame_index == animation_ended)
		{
			end(registry, entity_with_animation);
		}

		return next_frame_index;
	}

	auto DynamicSprite::jump_to_next_frame(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<BeginFrameIndex, EndFrameIndex, CurrentFrameIndex, AnimationMode, AnimationDirection>(entity_with_animation)));

		const auto& begin_frame_index = registry.get<const BeginFrameIndex>(entity_with_animation);
		const auto& end_frame_index = registry.get<const EndFrameIndex>(entity_with_animation);
		auto& current_frame_index = registry.get<CurrentFrameIndex>(entity_with_animation);
		const auto& mode = registry.get<const AnimationMode>(entity_with_animation);
		const auto& direction = registry.get<const AnimationDirection>(entity_with_animation);

		const auto next_frame_index = helper::DynamicSprite::jump_to_next_frame(begin_frame_index, end_frame_index, current_frame_index, mode, direction);

		// 标记为已结束
		if (next_frame_index == animation_ended)
		{
			end(registry, entity_with_animation);
		}

		return next_frame_index;
	}

	auto DynamicSprite::set_frame(entt::registry& registry, const entt::entity entity_with_animation, const index_type frame_index) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<BeginFrameIndex, EndFrameIndex, CurrentFrameIndex, CurrentFrameElapsed>(entity_with_animation)));

		const auto& begin_frame_index = registry.get<const BeginFrameIndex>(entity_with_animation);
		const auto& end_frame_index = registry.get<const EndFrameIndex>(entity_with_animation);
		auto& current_frame_index = registry.get<CurrentFrameIndex>(entity_with_animation);
		auto& current_frame_elapsed = registry.get<CurrentFrameElapsed>(entity_with_animation);

		return helper::DynamicSprite::set_frame(frame_index, begin_frame_index, end_frame_index, current_frame_index, current_frame_elapsed);
	}

	auto DynamicSprite::jump_to_frame(entt::registry& registry, const entt::entity entity_with_animation, const index_type frame_index) noexcept -> index_type
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<BeginFrameIndex, EndFrameIndex, CurrentFrameIndex>(entity_with_animation)));

		const auto& begin_frame_index = registry.get<const BeginFrameIndex>(entity_with_animation);
		const auto& end_frame_index = registry.get<const EndFrameIndex>(entity_with_animation);
		auto& current_frame_index = registry.get<CurrentFrameIndex>(entity_with_animation);

		return helper::DynamicSprite::jump_to_frame(frame_index, begin_frame_index, end_frame_index, current_frame_index);
	}

	auto DynamicSprite::set_mode(
		entt::registry& registry,
		const entt::entity entity_with_animation,
		const AnimationMode mode
	) noexcept -> void
	{
		registry.emplace_or_replace<AnimationMode>(entity_with_animation, mode);
	}

	auto DynamicSprite::get_mode(
		const entt::registry& registry,
		const entt::entity entity_with_animation
	) noexcept -> AnimationMode
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<AnimationMode>(entity_with_animation)));

		const auto& mode = registry.get<const AnimationMode>(entity_with_animation);

		return mode;
	}

	auto DynamicSprite::set_mode_loop(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		set_mode(registry, entity_with_animation, AnimationMode::LOOP);
	}

	auto DynamicSprite::set_mode_one_shot(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		set_mode(registry, entity_with_animation, AnimationMode::ONE_SHOT);
	}

	auto DynamicSprite::is_loop_mode(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return get_mode(registry, entity_with_animation) == AnimationMode::LOOP;
	}

	auto DynamicSprite::is_one_shot_mode(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return get_mode(registry, entity_with_animation) == AnimationMode::ONE_SHOT;
	}

	auto DynamicSprite::set_direction(
		entt::registry& registry,
		const entt::entity entity_with_animation,
		const AnimationDirection direction
	) noexcept -> void
	{
		registry.emplace_or_replace<AnimationDirection>(entity_with_animation, direction);
	}

	auto DynamicSprite::get_direction(
		const entt::registry& registry,
		const entt::entity entity_with_animation
	) noexcept -> AnimationDirection
	{
		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<AnimationDirection>(entity_with_animation)));

		const auto& direction = registry.get<const AnimationDirection>(entity_with_animation);

		return direction;
	}

	auto DynamicSprite::set_direction_forward(
		entt::registry& registry,
		const entt::entity entity_with_animation
	) noexcept -> void
	{
		set_direction(registry, entity_with_animation, AnimationDirection::FORWARD);
	}

	auto DynamicSprite::set_direction_backward(
		entt::registry& registry,
		const entt::entity entity_with_animation
	) noexcept -> void
	{
		set_direction(registry, entity_with_animation, AnimationDirection::BACKWARD);
	}

	auto DynamicSprite::is_forward_direction(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return get_direction(registry, entity_with_animation) == AnimationDirection::FORWARD;
	}

	auto DynamicSprite::is_backward_direction(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return get_direction(registry, entity_with_animation) == AnimationDirection::BACKWARD;
	}

	auto DynamicSprite::is_playing(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return not registry.any_of<Paused, Ended>(entity_with_animation);
	}

	auto DynamicSprite::is_paused(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return registry.all_of<Paused>(entity_with_animation);
	}

	auto DynamicSprite::pause(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.emplace_or_replace<Paused>(entity_with_animation);
	}

	auto DynamicSprite::resume(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.remove<Paused>(entity_with_animation);
	}

	auto DynamicSprite::is_ended(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> bool
	{
		return registry.all_of<Ended>(entity_with_animation);
	}

	auto DynamicSprite::end(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.emplace_or_replace<Ended>(entity_with_animation);
	}

	auto DynamicSprite::replay(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		registry.remove<Paused>(entity_with_animation);
		registry.remove<Ended>(entity_with_animation);

		if (is_forward_direction(registry, entity_with_animation))
		{
			set_frame(registry, entity_with_animation, 0);
		}
		else
		{
			const auto count = get_frame_count(registry, entity_with_animation);
			set_frame(registry, entity_with_animation, count - 1);
		}
	}
}
