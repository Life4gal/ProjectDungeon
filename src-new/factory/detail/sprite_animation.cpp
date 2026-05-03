// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/sprite_animation.hpp>

#include <ranges>

#include <manager/resource.hpp>

#include <blueprint/detail/sprite_animation.hpp>

#include <component/sprite_animation.hpp>

#include <factory/detail/sprite.hpp>

#include <entt/entity/registry.hpp>

namespace pd::factory::detail
{
	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::SpriteAnimation& sprite_animation) noexcept -> void
	{
		using namespace component;

		constexpr auto begin_frame_index = 0uz;

		// sprites
		auto& [sprites] = registry.emplace<sprite_animation::Sprites>(entity);
		sprites.reserve(std::ranges::min(sprite_animation.frames.size(), sprite_animation.durations_ms.size()));
		for (const auto [sprite, duration]: std::views::zip(sprite_animation.frames, sprite_animation.durations_ms))
		{
			auto texture = manager::Texture::load(std::string_view{sprite.texture});

			sprite_animation::Sprites::Sprite s
			{
					.texture = std::move(texture),
					.position = sf::Vector2i{sprite.x, sprite.y},
					.size = sf::Vector2i{sprite.width, sprite.height},
					.origin = sf::Vector2i{sprite.origin_x, sprite.origin_y},
					.duration_ms = duration
			};

			sprites.emplace_back(std::move(s));
		}
		// total
		registry.emplace<sprite_animation::Total>(entity, sprites.size());
		// timer
		registry.emplace<sprite_animation::Timer>(entity, sprite_animation::Timer{.duration = sf::milliseconds(sprite_animation.durations_ms[begin_frame_index]), .elapsed = sf::Time::Zero});
		// index
		registry.emplace<sprite_animation::Index>(entity, begin_frame_index);
		// mode
		registry.emplace<sprite_animation::Mode>(entity, sprite_animation.looping ? sprite_animation::Mode::LOOP : sprite_animation::Mode::ONE_SHOT);
		// direction
		registry.emplace<sprite_animation::Direction>(entity, sprite_animation.reversed ? sprite_animation::Direction::BACKWARD : sprite_animation::Direction::FORWARD);

		// sprite
		attach(registry, entity, sprite_animation.frames[begin_frame_index]);
	}
}
