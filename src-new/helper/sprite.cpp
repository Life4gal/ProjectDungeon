// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/sprite.hpp>

#include <manager/resource.hpp>

#include <component/sprite.hpp>
#include <component/sprite_effect.hpp>
#include <component/transform.hpp>
#include <component/tags.hpp>

#include <prometheus/meta/meta.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace pd::helper
{
	using namespace component;

	namespace
	{
		sf::RenderStates g_shared_states{};

		template<typename Tag>
		auto do_render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			const auto view = registry
					.view<
						state::InCameraArea,
						Tag,
						sprite::Texture,
						sprite::Position,
						sprite::Size,
						sprite::Origin,
						sprite_effect::Position,
						sprite_effect::Scale,
						sprite_effect::Rotation,
						sprite_effect::Color,
						transform::Position,
						transform::Rotation
					>(entt::exclude<state::Invisible>);

			for (const auto [
				     entity,
				     texture,
				     texture_position,
				     texture_size,
				     texture_origin,
				     effect_position,
				     effect_scale,
				     effect_rotation,
				     effect_color,
				     position,
				     rotation
			     ]: view.each())
			{
				const auto render_position = position.position + effect_position.extra;
				const auto render_scale = effect_scale.extra;
				const auto render_rotation = rotation.rotation + effect_rotation.extra;
				const auto render_color = effect_color.color;
				const auto render_origin = texture_origin.origin;

				if (texture.texture == manager::InvalidHandler)
				{
					SPDLOG_WARN(
						"无法渲染位于({:.1f}:{:.1f})的{}[0x{:08x}],无效的纹理资源句柄[{}]!",
						render_position.x,
						render_position.y,
						prometheus::meta::name_of<Tag>(),
						static_cast<std::uint32_t>(entity),
						manager::Texture::path_of(texture.texture)
					);

					// TODO: 如何做?默认纹理?还是直接销毁实体?

					continue;
				}

				const auto vertices = [&] noexcept -> auto
				{
					const auto texture_width = texture_size.size.x;
					const auto texture_height = texture_size.size.y;

					const auto texture_left = texture_position.position.x;
					const auto texture_top = texture_position.position.y;
					const auto texture_right = texture_left + texture_width;
					const auto texture_bottom = texture_top + texture_height;

					return std::array<sf::Vertex, 4>
					{{
							{.position = {0, 0}, .color = render_color, .texCoords = {texture_left, texture_top}},
							{.position = {texture_width, 0}, .color = render_color, .texCoords = {texture_right, texture_top}},
							{.position = {0, texture_height}, .color = render_color, .texCoords = {texture_left, texture_bottom}},
							{.position = {texture_width, texture_height}, .color = render_color, .texCoords = {texture_right, texture_bottom}},
					}};
				}();

				g_shared_states.texture = texture.texture.operator->();
				g_shared_states.transform = [&] noexcept -> sf::Transform
				{
					return sf::Transform{} //
					       .translate(render_position)
					       .rotate(render_rotation)
					       .scale(render_scale)
					       .translate(-render_origin);
				}();

				window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, g_shared_states);
			}
		}
	}

	auto Sprite::floor(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		do_render<tags::Floor>(registry, window);
	}

	auto Sprite::wall(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		do_render<tags::Wall>(registry, window);
	}

	auto Sprite::door(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		do_render<tags::Door>(registry, window);
	}

	auto Sprite::enemy(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		do_render<tags::Enemy>(registry, window);
	}

	auto Sprite::player(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		do_render<tags::Player>(registry, window);
	}

	auto Sprite::projectile(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		do_render<tags::Projectile>(registry, window);
	}
}
