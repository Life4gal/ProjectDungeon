// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/flip_animation.hpp>

#include <game/names.hpp>

#include <systems/world.hpp>
#include <systems/asset.hpp>

#include <components/transform.hpp>
#include <components/flip_animation.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace pd::systems
{
	auto FlipAnimation::do_loaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
		// 相关实体由Blueprint::initialize创建
		// registry.ctx().emplace_as<blueprint::animations_type>(Names::blueprint_animation);
	}

	auto FlipAnimation::do_initialized([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto FlipAnimation::do_unloaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto FlipAnimation::do_update(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;

		// 如果暂停游戏则无需更新动画
		if (World::is_pause(registry))
		{
			return;
		}

		for (const auto view = registry.view<FlipAnimationTimer, FlipAnimationIndex>();
		     const auto [entity, timer, index]: view.each())
		{
			timer.elapsed += delta;
			if (timer.elapsed < timer.duration)
			{
				continue;
			}

			timer.elapsed -= timer.duration;
			index.current += 1;

			if (index.current < index.total)
			{
				continue;
			}

			// 循环动画
			index.current %= index.total;
		}
	}

	auto FlipAnimation::do_render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		using namespace components;

		// 当前共用RenderStates
		static sf::RenderStates states{};

		for (const auto view = registry.view<
			     const Position,
			     const Scale,
			     const Rotation,
			     const FlipAnimationIndex,
			     const FlipAnimationFrames>();
		     const auto [entity, position, scale, rotation, animation_index, animation_frames]: view.each())
		{
			const auto& frame = animation_frames.frames[animation_index.current];
			const auto texture = Asset::texture(registry, frame.texture);

			if (not texture)
			{
				// todo: 默认纹理?
				SPDLOG_ERROR(
					"无法渲染位于({:.3f}:{:.3f})的实体[{}],纹理ID: {}",
					position.position.x,
					position.position.y,
					entt::to_integral(entity),
					frame.texture
				);
				continue;
			}

			const auto x = static_cast<float>(frame.frame_x);
			const auto y = static_cast<float>(frame.frame_y);
			const auto width = static_cast<float>(frame.frame_width);
			const auto height = static_cast<float>(frame.frame_height);
			// todo: 原点一定是中心点吗?
			const auto origin = sf::Vector2f{width, height} / 2.f;

			const auto vertices = [&] noexcept -> auto
			{
				// todo: 能不能控制颜色?
				// 可以做一些闪烁效果
				constexpr auto color = sf::Color::White;

				const auto right = x + width;
				const auto bottom = y + height;

				return std::array<sf::Vertex, 4>
				{{
						{.position = {0, 0}, .color = color, .texCoords = {x, y}},
						{.position = {width, 0}, .color = color, .texCoords = {right, y}},
						{.position = {0, height}, .color = color, .texCoords = {x, bottom}},
						{.position = {width, height}, .color = color, .texCoords = {right, bottom}},
				}};
			}();

			states.texture = texture.operator->();
			states.transform = [&] noexcept -> sf::Transform
			{
				sf::Transform t{};
				t.translate(position.position);
				t.scale(scale.scale);
				t.rotate(rotation.rotation);
				t.translate(-origin);

				return t;
			}();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, states);
		}
	}

	auto FlipAnimation::get(entt::registry& registry, const std::string_view name) noexcept -> blueprint::flip_animation_frames_view_type
	{
		auto& animations = registry.ctx().get<blueprint::flip_animations_type>(Names::blueprint_flip_animation);

		if (const auto it = animations.find(name);
			it != animations.end())
		{
			return it->second;
		}

		// todo: 备用动画?
		return animations.begin()->second;
	}
}
