// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/animation.hpp>

#include <config/dungeon.hpp>

#include <components/animation.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto Animation::attach(entt::registry& registry, const entt::entity entity, const config::Animation& animation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(not registry.all_of<animation::Animation>(entity));

		const auto& first_frame = animation.frames[0];
		const auto total_frames = animation.frames.size();

		registry.emplace_or_replace<animation::Animation>(entity, std::cref(animation));
		registry.emplace_or_replace<animation::Timer>(entity, animation::Timer{.duration = sf::milliseconds(first_frame.duration_ms), .elapsed = sf::Time::Zero});
		registry.emplace_or_replace<animation::Index>(entity, animation::Index{.total = total_frames, .current = 0});

		if (animation.looping)
		{
			registry.emplace_or_replace<animation::Looping>(entity);
		}
		else
		{
			registry.emplace_or_replace<animation::Ended>(entity);
		}

		// pause标签按需调用接口添加
	}

	auto Animation::deattach(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		using namespace components;

		registry.remove<animation::Animation>(entity_with_animation);
		registry.remove<animation::Timer>(entity_with_animation);
		registry.remove<animation::Index>(entity_with_animation);
		registry.remove<animation::Looping>(entity_with_animation);
		registry.remove<animation::Paused>(entity_with_animation);
		registry.remove<animation::Ended>(entity_with_animation);
	}

	auto Animation::get_texture_width(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> int
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		const auto& [animation] = registry.get<const animation::Animation>(entity_with_animation);

		return animation.get().texture_width;
	}

	auto Animation::get_texture_height(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> int
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		const auto& [animation] = registry.get<const animation::Animation>(entity_with_animation);

		return animation.get().texture_height;
	}

	auto Animation::get_origin_x(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> int
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		const auto& [animation] = registry.get<const animation::Animation>(entity_with_animation);

		return animation.get().origin_x;
	}

	auto Animation::get_origin_y(const entt::registry& registry, const entt::entity entity_with_animation) noexcept -> int
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		const auto& [animation] = registry.get<const animation::Animation>(entity_with_animation);

		return animation.get().origin_y;
	}

	auto Animation::set_next_frame(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> std::size_t
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		const auto next_frame_index = jump_to_next_frame(registry, entity_with_animation);

		// 设置帧计时器
		const auto& animation = registry.get<const animation::Animation>(entity_with_animation).animation.get();
		const auto& next_frame = animation.frames[next_frame_index];

		auto& [duration, elapsed] = registry.get<animation::Timer>(entity_with_animation);

		duration = sf::milliseconds(next_frame.duration_ms);
		elapsed = sf::Time::Zero;

		return next_frame_index;
	}

	auto Animation::jump_to_next_frame(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> std::size_t
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		const auto& [total, current] = registry.get<const animation::Index>(entity_with_animation);
		const auto revered = registry.all_of<animation::Reversed>(entity_with_animation);
		const auto loop = registry.all_of<animation::Looping>(entity_with_animation);

		// 如果到了最后一帧(或第一帧)且不是循环动画则标记为已结束
		if (const auto end_frame_index = revered ? 0 : total - 1;
			current == end_frame_index and not loop)
		{
			registry.emplace<animation::Ended>(entity_with_animation);
			return animation_ended;
		}

		const auto next_frame_index = revered ? (current == 0 ? total - 1 : current - 1) : (current + 1) % total;
		jump_to_frame(registry, entity_with_animation, next_frame_index);
		return next_frame_index;
	}

	auto Animation::set_frame(entt::registry& registry, const entt::entity entity_with_animation, const std::size_t frame_index) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		jump_to_frame(registry, entity_with_animation, frame_index);

		// 设置帧计时器
		const auto& animation = registry.get<const animation::Animation>(entity_with_animation).animation.get();
		const auto& target_frame = animation.frames[frame_index];

		auto& [duration, elapsed] = registry.get<animation::Timer>(entity_with_animation);

		duration = sf::milliseconds(target_frame.duration_ms);
		elapsed = sf::Time::Zero;
	}

	auto Animation::jump_to_frame(entt::registry& registry, const entt::entity entity_with_animation, const std::size_t frame_index) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		const auto& [total, current] = registry.get<const animation::Index>(entity_with_animation);

		// 假定外部输入不会出错?
		PROMETHEUS_PLATFORM_ASSUME(frame_index < total);

		// 设置当前帧索引
		registry.emplace_or_replace<animation::Index>(entity_with_animation, animation::Index{.total = total, .current = frame_index});
	}

	auto Animation::looping(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		registry.emplace_or_replace<animation::Looping>(entity_with_animation);

		// 如果非循环动画之前已经播放完毕,则需要移除已结束的标记
		registry.remove<animation::Ended>(entity_with_animation);
	}

	auto Animation::unlooping(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		registry.remove<animation::Looping>(entity_with_animation);
	}

	auto Animation::pause(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		registry.emplace_or_replace<animation::Paused>(entity_with_animation);
	}

	auto Animation::unpause(entt::registry& registry, const entt::entity entity_with_animation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<animation::Animation>(entity_with_animation));

		registry.remove<animation::Paused>(entity_with_animation);
	}
}
