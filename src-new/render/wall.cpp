// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/wall.hpp>

#include <manager/resource.hpp>

#include <component/wall.hpp>

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
					sprite::TexturePosition,
					sprite::TextureSize,
					sprite::TextureOrigin,
					sprite::RenderPositionOffset,
					sprite::RenderScale,
					sprite::RenderRotation,
					sprite::RenderColor,
					position::World
				>(entt::exclude<state::Invisible>);

		for (const auto [entity, texture, texture_position, texture_size, texture_origin, render_position_offset, render_scale, render_rotation, render_color, position]: view.each())
		{
			const auto render_position = position.position + render_position_offset.offset;

			if (texture.texture == manager::InvalidHandler)
			{
				SPDLOG_WARN(
					"无法渲染位于({:.1f}:{:.1f})的墙壁[0x{:08x}],无效的纹理资源句柄[{}]!",
					render_position.x,
					render_position.y,
					static_cast<std::uint32_t>(entity),
					manager::Texture::path_of(texture.texture)
				);

				// TODO: 如何做?默认纹理?还是直接销毁实体?

				continue;
			}

			const auto vertices = [&] noexcept -> auto
			{
				const auto x = texture_position.position.x;
				const auto y = texture_position.position.y;
				const auto width = texture_size.size.x;
				const auto height = texture_size.size.y;

				const auto right = x + width;
				const auto bottom = y + height;

				return std::array<sf::Vertex, 4>
				{{
						{.position = {0, 0}, .color = render_color.color, .texCoords = {x, y}},
						{.position = {width, 0}, .color = render_color.color, .texCoords = {right, y}},
						{.position = {0, height}, .color = render_color.color, .texCoords = {x, bottom}},
						{.position = {width, height}, .color = render_color.color, .texCoords = {right, bottom}},
				}};
			}();

			g_shared_states.texture = texture.texture.operator->();
			g_shared_states.transform = [&] noexcept -> sf::Transform
			{
				const auto scale_x = render_scale.scale.x;
				const auto scale_y = render_scale.scale.y;
				const auto origin_x = texture_origin.origin.x * scale_x;
				const auto origin_y = texture_origin.origin.y * scale_y;

				return sf::Transform{}
				       //
				       .translate(render_position)
				       .rotate(render_rotation.rotation)
				       .scale({scale_x, scale_y})
				       .translate({-origin_x, -origin_y});
			}();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, g_shared_states);
		}
	}
}
