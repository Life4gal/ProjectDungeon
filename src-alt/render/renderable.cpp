// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/renderable.hpp>

#include <manager/asset.hpp>

#include <components/tags.hpp>
#include <components/transform.hpp>
#include <components/renderable.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace pd::render
{
	auto renderable(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		using namespace components;

		const auto group = registry.group<renderable::RenderLayer>(
			entt::get<
				renderable::Texture,
				renderable::Rect,
				renderable::Origin,
				renderable::Color,
				transform::Position,
				transform::Scale,
				transform::Rotation
			>,
			entt::exclude<
				// 排除禁用
				tags::disabled,
				// 排除不可见
				renderable::Invisible>
		);

		// 检查是否需要排序
		if (registry.ctx().contains<renderable::SortRequired>())
		{
			// todo: 对于同一渲染层的实体我们最好也有一种确定的排序方式,避免多个实体在同一层时的渲染顺序不确定(可能会导致闪烁)
			group.sort<renderable::RenderLayer>(
				[](const renderable::RenderLayer lhs, const renderable::RenderLayer rhs) noexcept -> bool
				{
					return std::to_underlying(lhs) < std::to_underlying(rhs);
				}
			);
		}

		// 共用RenderStates
		static sf::RenderStates states{};

		for (const auto [entity, render_layer, texture, rect, origin, color, position, scale, rotation]: group.each())
		{
			const auto texture_resource = manager::Texture::get(texture.id);
			if (not texture_resource)
			{
				// todo: 有没有默认/备用纹理?
				SPDLOG_ERROR(
					"无法渲染位于({:.1f}:{:.1f})的实体[0x{:08x}],纹理资源[0x{:08x}]不存在",
					position.position.x,
					position.position.y,
					entt::to_integral(entity),
					std::to_underlying(texture.id)
				);
				continue;
			}

			const auto vertices = [&] noexcept -> auto
			{
				const auto x = static_cast<float>(rect.x);
				const auto y = static_cast<float>(rect.y);
				const auto width = static_cast<float>(rect.width);
				const auto height = static_cast<float>(rect.height);

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
				const auto origin_x = static_cast<float>(origin.x);
				const auto origin_y = static_cast<float>(origin.y);

				sf::Transform transform{};
				transform.translate(position.position);
				transform.scale(scale.scale);
				transform.rotate(rotation.rotation);

				transform.translate({-origin_x, -origin_y});

				return transform;
			}();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, states);
		}
	}
}
