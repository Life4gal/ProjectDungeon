// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/collect_render_item.hpp>

#include <component/renderer.hpp>
#include <component/render.hpp>
#include <component/transform.hpp>
#include <component/tags.hpp>

#include <entt/entt.hpp>

namespace pd::render
{
	using namespace component;

	auto collect_render_item(entt::registry& registry) noexcept -> void
	{
		namespace r = component::render;
		namespace rss = component::render::static_sprite;

		// 清空之前的渲染单元集
		auto& [set] = registry.ctx().get<renderer::RenderItemSet>();
		set.clear();

		const auto view = registry
				.view<
					state::InCameraArea,
					r::RenderLayer,
					rss::Texture,
					rss::UvPosition,
					rss::UvSize,
					rss::Pivot,
					transform::Position,
					transform::Scale,
					transform::Rotation
				>(entt::exclude<state::DisableRender>);

		for (const auto [entity, render_layer, texture, texture_position, texture_size, texture_pivot, position, scale, rotation]: view.each())
		{
			set[entity] =
			{
					.position = position.position,
					.scale = scale.scale,
					.rotation = rotation.rotation,
					.render_layer = render_layer,
					.texture = texture.texture,
					.uv_position = texture_position.position,
					.uv_size = texture_size.size,
					.pivot = texture_pivot.pivot,
					.color = sf::Color::White,
			};
		}
	}
}
