// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/render.hpp>

#include <manager/resource.hpp>

#include <component/render.hpp>
#include <component/renderer.hpp>

#include <entt/entt.hpp>
#include <SFML/System/Clock.hpp>

namespace pd::factory::detail
{
	using namespace component;

	namespace
	{
		sf::Clock g_clock;
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::StaticSprite& static_sprite, const blueprint::RenderLayer render_layer) noexcept -> void
	{
		namespace rss = render::static_sprite;
		namespace res = render_effect::sprite;

		// 渲染层
		registry.emplace<render::RenderLayer>(entity, render_layer);
		// 生成时间
		registry.emplace<render::SpawnTime>(entity, g_clock.getElapsedTime());

		// 需要重新排序
		registry.ctx().emplace<renderer::SortRequired>();

		// ====================
		// SPRITE
		// ====================

		auto texture = manager::Texture::load(std::filesystem::path{static_sprite.texture});

		registry.emplace<rss::Texture>(entity, std::move(texture));
		registry.emplace<rss::Position>(entity, sf::Vector2f{static_sprite.position.x, static_sprite.position.y});
		registry.emplace<rss::Size>(entity, sf::Vector2f{static_sprite.size.width, static_sprite.size.height});
		registry.emplace<rss::Origin>(entity, sf::Vector2f{static_sprite.origin.x, static_sprite.origin.y});

		// ====================
		// EFFECT
		// ====================

		registry.emplace<res::Position>(entity, sf::Vector2f{0, 0});
		registry.emplace<res::Scale>(entity, sf::Vector2f{1, 1});
		registry.emplace<res::Rotation>(entity, sf::degrees(0));
		registry.emplace<res::Color>(entity, sf::Color::White);
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::DynamicSprite& dynamic_sprite, const blueprint::RenderLayer render_layer) noexcept -> void
	{
		namespace rds = render::dynamic_sprite;

		constexpr auto begin_frame_index = 0uz;

		// frames
		{
			auto& [frames] = registry.emplace<rds::Frames>(entity);
			frames.reserve(dynamic_sprite.frames.size());

			for (const auto& [texture, position]: dynamic_sprite.frames)
			{
				auto texture_handler = manager::Texture::load(std::string_view{texture});

				frames.emplace_back(std::move(texture_handler), sf::Vector2f{position.x, position.y});
			}
		}
		// frames_count
		registry.emplace<rds::FramesCount>(entity, dynamic_sprite.frames.size());
		// duration
		registry.emplace<rds::Duration>(entity, sf::milliseconds(dynamic_sprite.duration_ms));
		// timer
		registry.emplace<rds::Timer>(entity, sf::Time::Zero);
		// index
		registry.emplace<rds::Index>(entity, begin_frame_index);
		// mode
		registry.emplace<rds::Mode>(entity, dynamic_sprite.looping ? rds::Mode::LOOP : rds::Mode::ONE_SHOT);
		// direction
		registry.emplace<rds::Direction>(entity, dynamic_sprite.reversed ? rds::Direction::BACKWARD : rds::Direction::FORWARD);

		// static sprite
		const auto& [texture, position] = dynamic_sprite.frames[begin_frame_index];
		const blueprint::StaticSprite static_sprite
		{
				.texture = texture,
				.position = position,
				.size = dynamic_sprite.size,
				.origin = dynamic_sprite.origin,
		};
		attach(registry, entity, static_sprite, render_layer);
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Sprite& sprite) noexcept -> void
	{
		std::visit(
			[&](const auto& s) noexcept -> void
			{
				attach(registry, entity, s, sprite.render_layer);
			},
			sprite.sprite
		);
	}
}
