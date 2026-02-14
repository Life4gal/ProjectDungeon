// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/debug.hpp>

#include <components/transform.hpp>
#include <components/animation.hpp>

#include <systems/animation.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Debug::do_loaded(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		const auto entity = registry.create();

		// transform
		{
			registry.emplace<Position>(entity, sf::Vector2f{500, 500});
			registry.emplace<Scale>(entity, sf::Vector2f{10, 10});
			registry.emplace<Rotation>(entity, sf::degrees(0));
		}

		// animation
		{
			const auto animation = Animation::get(registry, "AntleredRascal");
			const auto& begin_frame = animation->frames.front();

			registry.emplace<AnimationTimer>(entity, AnimationTimer{.duration = begin_frame.duration, .elapsed = sf::Time::Zero});
			registry.emplace<AnimationIndex>(entity, AnimationIndex{.total = animation->frames.size(), .current = 0});
			registry.emplace<AnimationView>(entity, animation);

			if (animation->looping)
			{
				registry.emplace<AnimationLooping>(entity);
			}
		}
	}

	auto Debug::do_initialized([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Debug::do_unloaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Debug::do_update([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		// 什么也不做
	}

	auto Debug::do_render([[maybe_unused]] entt::registry& registry, [[maybe_unused]] sf::RenderWindow& window) noexcept -> void
	{
		// 什么也不做
	}
}
