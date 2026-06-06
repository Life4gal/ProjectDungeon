// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/build_render_queue.hpp>

#include <array>
#include <algorithm>
#include <ranges>

#include <component/renderer.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	namespace
	{
		auto append_quad(std::vector<sf::Vertex>& vertices, const renderer::RenderSet::Item& item) noexcept -> void
		{
			const auto texture_width = item.uv_size.x;
			const auto texture_height = item.uv_size.y;

			const auto texture_left = item.uv_position.x;
			const auto texture_top = item.uv_position.y;
			const auto texture_right = texture_left + item.uv_size.x;
			const auto texture_bottom = texture_top + item.uv_size.y;

			const auto offset = item.effect.has_value() ? item.effect->offset : sf::Vector2f{0, 0};
			const auto scale = item.effect.has_value() ? item.effect->scale : sf::Vector2f{1, 1};
			const auto rotation = item.effect.has_value() ? item.effect->rotation : sf::radians(0);
			const auto color = item.effect.has_value() ? item.effect->color : sf::Color::White;

			const auto cos_rot = std::cos(rotation.asRadians());
			const auto sin_rot = std::sin(rotation.asRadians());

			// 归一化uv
			// 除以整张纹理大小,而不是该子纹理大小
			// const auto full_texture_size = sf::Vector2f{item.texture->getSize()};
			const auto coordinate_left = texture_left; // / full_texture_size.x;
			const auto coordinate_top = texture_top; // / full_texture_size.y;
			const auto coordinate_right = texture_right; // / full_texture_size.x;
			const auto coordinate_bottom = texture_bottom; // / full_texture_size.y;

			const std::array<sf::Vector2f, 4> coordinates
			{{
					{coordinate_left, coordinate_top},
					{coordinate_right, coordinate_top},
					{coordinate_left, coordinate_bottom},
					{coordinate_right, coordinate_bottom},
			}};

			// local-space
			const std::array<sf::Vector2f, 4> locals
			{{
					{0, 0},
					{texture_width, 0},
					{0, texture_height},
					{texture_width, texture_height},
			}};

			// world-space
			const auto worlds = [&] noexcept -> std::array<sf::Vector2f, 4>
			{
				std::array<sf::Vector2f, 4> ws;

				for (const auto [local, world]: std::views::zip(locals, ws))
				{
					auto p = local - item.pivot;
					p.x *= scale.x;
					p.y *= scale.y;

					const auto rx = p.x * cos_rot - p.y * sin_rot;
					const auto ry = p.x * sin_rot + p.y * cos_rot;

					world = item.position + offset + sf::Vector2f{rx, ry};
				}

				return ws;
			}();

			// two triangles: v0-v1-v2, v1-v3-v2
			vertices.push_back({.position = worlds[0], .color = color, .texCoords = coordinates[0]});
			vertices.push_back({.position = worlds[1], .color = color, .texCoords = coordinates[1]});
			vertices.push_back({.position = worlds[2], .color = color, .texCoords = coordinates[2]});
			vertices.push_back({.position = worlds[1], .color = color, .texCoords = coordinates[1]});
			vertices.push_back({.position = worlds[3], .color = color, .texCoords = coordinates[3]});
			vertices.push_back({.position = worlds[2], .color = color, .texCoords = coordinates[2]});
		}
	}

	auto build_render_queue(entt::registry& registry) noexcept -> void
	{
		// 渲染单元集
		const auto& [set] = registry.ctx().get<const renderer::RenderSet>();
		// 所有单元
		const auto set_values = set | std::views::values;
		// 复制所有单元
		std::vector<renderer::RenderSet::Item> items{set_values.begin(), set_values.end()};

		// 排序:
		// RenderLayer
		// texture
		// shader
		// Z-ORDER (transform::Position::y)
		std::ranges::sort(
			items,
			[](const renderer::RenderSet::Item& lhs, const renderer::RenderSet::Item& rhs) noexcept -> bool
			{
				if (lhs.render_layer != rhs.render_layer)
				{
					return std::to_underlying(lhs.render_layer) < std::to_underlying(rhs.render_layer);
				}

				// 必定有纹理
				PROMETHEUS_PLATFORM_ASSUME(lhs.texture != manager::InvalidHandler);
				PROMETHEUS_PLATFORM_ASSUME(rhs.texture != manager::InvalidHandler);
				// 纹理排序无所谓吧?
				if (lhs.texture != rhs.texture)
				{
					return lhs.texture.operator->() < rhs.texture.operator->();
				}

				// 不一定有着色器
				if (lhs.shader != manager::InvalidHandler or rhs.shader != manager::InvalidHandler)
				{
					// 着色器排序无所谓吧?
					return lhs.shader == manager::InvalidHandler;
				}

				return lhs.position.y < rhs.position.y;
			}
		);

		// 清空之前的渲染队列
		auto& [queue] = registry.ctx().get<renderer::RenderQueue>();
		queue.clear();

		for (const auto& item: items)
		{
			const auto new_batch = [&] noexcept -> bool
			{
				if (queue.empty())
				{
					return true;
				}

				const auto& last_batch = queue.back();

				if (last_batch.texture != item.texture)
				{
					return true;
				}

				if (last_batch.shader != item.shader)
				{
					return true;
				}

				return false;
			}();

			if (new_batch)
			{
				queue.push_back(
					{
							.texture = item.texture,
							.shader = item.shader,
							.vertices = {},
					}
				);
			}

			append_quad(queue.back().vertices, item);
		}
	}
}
