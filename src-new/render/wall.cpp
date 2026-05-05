// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/wall.hpp>

#include <manager/resource.hpp>

#include <component/wall.hpp>
#include <component/state.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	namespace
	{
		sf::RenderStates g_shared_states{};
	}

	auto wall(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		const auto view = registry
				.view<
					state::InCameraArea,
					tags::Wall,
					sprite::Texture,
					sprite::Position,
					sprite::Size,
					sprite::Origin,
					sprite::Color,
					sprite::Scale,
					transform::Position,
					transform::Scale,
					transform::Rotation
				>(entt::exclude<state::Invisible>);

		for (const auto [entity, sprite_texture, sprite_position, sprite_size, sprite_origin, sprite_color, sprite_scale, transform_position, transform_scale, transform_rotation]: view.each())
		{
			if (sprite_texture.texture == manager::InvalidHandler)
			{
				SPDLOG_WARN(
					"无法渲染位于({:.1f}:{:.1f})的墙壁[0x{:08x}],无效的纹理资源句柄[{}]!",
					transform_position.position.x,
					transform_position.position.y,
					static_cast<std::uint32_t>(entity),
					manager::Texture::path_of(sprite_texture.texture)
				);

				// TODO: 如何做?默认纹理?还是直接销毁实体?

				continue;
			}

			const auto vertices = [&] noexcept -> auto
			{
				const auto x = static_cast<float>(sprite_position.position.x);
				const auto y = static_cast<float>(sprite_position.position.y);
				const auto width = static_cast<float>(sprite_size.size.x);
				const auto height = static_cast<float>(sprite_size.size.y);

				const auto right = x + width;
				const auto bottom = y + height;

				return std::array<sf::Vertex, 4>
				{{
						{.position = {0, 0}, .color = sprite_color.color, .texCoords = {x, y}},
						{.position = {width, 0}, .color = sprite_color.color, .texCoords = {right, y}},
						{.position = {0, height}, .color = sprite_color.color, .texCoords = {x, bottom}},
						{.position = {width, height}, .color = sprite_color.color, .texCoords = {right, bottom}},
				}};
			}();

			g_shared_states.texture = sprite_texture.texture.operator->();
			g_shared_states.transform = [&] noexcept -> sf::Transform
			{
				// 缩放叠加
				const auto scale_x = sprite_scale.scale.x * transform_scale.scale.x;
				const auto scale_y = sprite_scale.scale.y * transform_scale.scale.y;
				const auto origin_x = static_cast<float>(sprite_origin.origin.x) * scale_x;
				const auto origin_y = static_cast<float>(sprite_origin.origin.y) * scale_y;

				return sf::Transform{}
				       //
				       .translate(transform_position.position)
				       .rotate(transform_rotation.rotation)
				       .scale({scale_x, scale_y})
				       .translate({-origin_x, -origin_y});
			}();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, g_shared_states);
		}
	}
}
