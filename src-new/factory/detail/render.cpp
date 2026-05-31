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

		template<typename StaticSpriteLike>
			requires(std::is_same_v<StaticSpriteLike, blueprint::StaticSprite> or std::is_same_v<StaticSpriteLike, blueprint::DynamicSprite::Frame>)
		auto do_attach(entt::registry& registry, const entt::entity entity, const StaticSpriteLike& static_sprite, const blueprint::RenderLayer render_layer) noexcept -> void
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
			registry.emplace<rss::UvPosition>(entity, sf::Vector2f{static_sprite.uv_position.x, static_sprite.uv_position.y});
			registry.emplace<rss::UvSize>(entity, sf::Vector2f{static_sprite.uv_size.width, static_sprite.uv_size.height});
			registry.emplace<rss::Pivot>(entity, sf::Vector2f{static_sprite.pivot.x, static_sprite.pivot.y});

			// ====================
			// EFFECT
			// ====================

			registry.emplace<res::Position>(entity, sf::Vector2f{0, 0});
			registry.emplace<res::Scale>(entity, sf::Vector2f{1, 1});
			registry.emplace<res::Rotation>(entity, sf::degrees(0));
			registry.emplace<res::Color>(entity, sf::Color::White);
		}
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::StaticSprite& static_sprite, const blueprint::RenderLayer render_layer) noexcept -> void
	{
		do_attach(registry, entity, static_sprite, render_layer);
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::DynamicSprite& dynamic_sprite, const blueprint::RenderLayer render_layer) noexcept -> void
	{
		namespace rds = render::dynamic_sprite;

		constexpr auto begin_frame_index = 0uz;

		// frames
		{
			auto& [frames] = registry.emplace<rds::Frames>(entity);
			frames.reserve(dynamic_sprite.frames.size());

			for (const auto& [texture, position, size, pivot, duration_ms]: dynamic_sprite.frames)
			{
				auto texture_handler = manager::Texture::load(std::string_view{texture});

				rds::Frames::Frame frame
				{
						.texture = std::move(texture_handler),
						.position = {position.x, position.y},
						.size = {size.width, size.height},
						.pivot = {pivot.x, pivot.y},
						.duration = sf::milliseconds(duration_ms),
				};

				frames.push_back(std::move(frame));
			}
		}
		// frames_count
		registry.emplace<rds::FramesCount>(entity, dynamic_sprite.frames.size());
		// timer
		registry.emplace<rds::Timer>(entity, sf::Time::Zero);
		// index
		registry.emplace<rds::Index>(entity, begin_frame_index);
		// mode
		registry.emplace<rds::Mode>(entity, dynamic_sprite.looping ? rds::Mode::LOOP : rds::Mode::ONE_SHOT);
		// direction
		registry.emplace<rds::Direction>(entity, dynamic_sprite.reversed ? rds::Direction::BACKWARD : rds::Direction::FORWARD);
		// paused
		if (dynamic_sprite.pause)
		{
			registry.emplace<rds::Paused>(entity);
		}

		// static sprite
		const auto& begin_frame = dynamic_sprite.frames[begin_frame_index];
		do_attach(registry, entity, begin_frame, render_layer);
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
