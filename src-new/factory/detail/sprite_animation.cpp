// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/sprite_animation.hpp>

#include <manager/resource.hpp>

#include <blueprint/detail/sprite_animation.hpp>

#include <component/sprite_animation.hpp>

#include <factory/detail/sprite.hpp>

#include <entt/entt.hpp>

namespace pd::factory::detail
{
	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::SpriteAnimation& sprite_animation) noexcept -> void
	{
		using namespace component;

		constexpr auto begin_frame_index = 0uz;

		// frames
		{
			auto& [frames] = registry.emplace<sprite_animation::Frames>(entity);
			frames.reserve(sprite_animation.frames.size());

			for (const auto& [texture, position]: sprite_animation.frames)
			{
				auto texture_handler = manager::Texture::load(std::string_view{texture});

				frames.emplace_back(std::move(texture_handler), sf::Vector2f{position.x, position.y});
			}
		}
		// frames_count
		registry.emplace<sprite_animation::FramesCount>(entity, sprite_animation.frames.size());
		// duration
		registry.emplace<sprite_animation::Duration>(entity, sf::milliseconds(sprite_animation.duration_ms));
		// timer
		registry.emplace<sprite_animation::Timer>(entity, sf::Time::Zero);
		// index
		registry.emplace<sprite_animation::Index>(entity, begin_frame_index);
		// mode
		registry.emplace<sprite_animation::Mode>(entity, sprite_animation.looping ? sprite_animation::Mode::LOOP : sprite_animation::Mode::ONE_SHOT);
		// direction
		registry.emplace<sprite_animation::Direction>(entity, sprite_animation.reversed ? sprite_animation::Direction::BACKWARD : sprite_animation::Direction::FORWARD);

		// sprite
		const auto& [texture,position] = sprite_animation.frames[begin_frame_index];
		const blueprint::Sprite sprite
		{
				.texture = texture,
				.position = position,
				.size = sprite_animation.size,
				.origin = sprite_animation.origin,
				.scale = sprite_animation.scale,
		};
		attach(registry, entity, sprite);
	}
}
