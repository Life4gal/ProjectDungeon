// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/render.hpp>

#include <manager/resource.hpp>

#include <component/render.hpp>
#include <component/renderer.hpp>
#include <component/transform.hpp>
#include <component/state.hpp>

#include <prometheus/meta/meta.hpp>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace pd::render
{
	using namespace component;

	namespace
	{
		sf::RenderStates g_shared_states{};
	}

	auto render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		namespace rss = component::render::static_sprite;
		namespace res = render_effect::sprite;

		const auto group = registry.group<component::render::RenderLayer, component::render::SpawnTime>(
			entt::get<
				state::sprite::Awake,
				rss::Texture,
				rss::UvPosition,
				rss::UvSize,
				rss::Pivot,
				res::Position,
				res::Scale,
				res::Rotation,
				res::Color,
				transform::Position,
				transform::Rotation
			>,
			entt::exclude<state::sprite::Invisible>
		);

		// 检查是否需要排序
		if (registry.ctx().contains<renderer::SortRequired>())
		{
			// TODO: 需不需要加入Z-ORDER?
			//  position::y越小的先渲染
			group.sort<component::render::RenderLayer, component::render::SpawnTime>(
				[](
			const std::tuple<component::render::RenderLayer&, component::render::SpawnTime&>& lhs,
			const std::tuple<component::render::RenderLayer&, component::render::SpawnTime&>& rhs
		) noexcept -> bool
				{
					const auto& [lhs_rl, lhs_st] = lhs;
					const auto& [rhs_rl, rhs_st] = rhs;

					if (std::to_underlying(lhs_rl) < std::to_underlying(rhs_rl))
					{
						return true;
					}

					if (lhs_rl == rhs_rl)
					{
						return lhs_st.time < rhs_st.time;
					}

					return false;
				}
			);

			registry.ctx().erase<renderer::SortRequired>();
		}

		for (const auto [
			     entity,
			     render_layer,
			     spawn_time,
			     texture,
			     texture_position,
			     texture_size,
			     texture_pivot,
			     effect_position,
			     effect_scale,
			     effect_rotation,
			     effect_color,
			     position,
			     rotation
		     ]: group.each())
		{
			const auto render_position = position.position + effect_position.extra;
			const auto render_scale = effect_scale.extra;
			const auto render_rotation = rotation.rotation + effect_rotation.extra;
			const auto render_color = effect_color.color;
			const auto render_pivot = texture_pivot.pivot;

			if (texture.texture == manager::InvalidHandler)
			{
				SPDLOG_WARN(
					"无法渲染位于({:.1f}:{:.1f})的实体(0x{:08x}),无效的纹理资源句柄[{}]!",
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
				       .translate(-render_pivot);
			}();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, g_shared_states);
		}
	}
}
