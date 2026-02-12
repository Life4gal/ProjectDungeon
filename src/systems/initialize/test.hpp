#pragma once

#include <components/transform.hpp>
#include <components/animation.hpp>

#include <ctx/animation.hpp>

#include <entt/entt.hpp>

namespace pd::systems::initialize
{
	inline auto test(entt::registry& registry) noexcept -> void
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
			const auto& animations = ctx::Animations::get(registry);
			const auto& [name, frames] = *animations.begin();
			const auto& begin_frame = frames.front();

			registry.emplace<AnimationTimer>(entity, AnimationTimer{.duration = begin_frame.duration, .elapsed = sf::Time::Zero});
			registry.emplace<AnimationIndex>(entity, AnimationIndex{.total = frames.size(), .current = 0});
			registry.emplace<Animation>(entity, Animation{.frames = frames});
		}
	}
}
