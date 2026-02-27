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
}
