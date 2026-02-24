// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/render/entity.hpp>

#include <components/transform.hpp>
#include <components/render.hpp>

#include <systems/helper/texture_manager.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace pd::systems::render
{
	auto entity(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		using namespace components;

		// 当前共用RenderStates
		static sf::RenderStates states{};

		for (const auto view = registry.view<
			     const transform::Position,
			     const transform::Scale,
			     const transform::Rotation,
			     const components::render::Texture,
			     const components::render::Color>();
		     const auto [entity, position, scale, rotation, texture, color]: view.each())
		{
			// 获取纹理资源
			const auto texture_resource = helper::TextureManager::get(registry, texture.texture_path);

			if (not texture_resource)
			{
				// todo: 默认纹理?
				SPDLOG_ERROR(
					"无法渲染位于({:.1f}:{:.1f})的实体[0x{:08x}],纹理[{}]不存在",
					position.position.x,
					position.position.y,
					entt::to_integral(entity),
					texture.texture_path.string()
				);
				continue;
			}

			const auto x = static_cast<float>(texture.frame_x);
			const auto y = static_cast<float>(texture.frame_y);
			const auto width = static_cast<float>(texture.frame_width);
			const auto height = static_cast<float>(texture.frame_height);
			// FIXME: 总是假定原点为正中心(width/2, height/2)
			const auto origin = sf::Vector2f{width, height} / 2.f;

			const auto vertices = [&] noexcept -> auto
			{
				const auto right = x + width;
				const auto bottom = y + height;

				return std::array<sf::Vertex, 4>
				{{
						{.position = {0, 0}, .color = color.color, .texCoords = {x, y}},
						{.position = {width, 0}, .color = color.color, .texCoords = {right, y}},
						{.position = {0, height}, .color = color.color, .texCoords = {x, bottom}},
						{.position = {width, height}, .color = color.color, .texCoords = {right, bottom}},
				}};
			}();

			states.texture = texture_resource.operator->();
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
}
