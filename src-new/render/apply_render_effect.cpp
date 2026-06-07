// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/apply_render_effect.hpp>

#include <component/renderer.hpp>
#include <component/render_effect.hpp>
#include <component/tags.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::render
{
	using namespace component;

	auto apply_render_effect(entt::registry& registry) noexcept -> void
	{
		auto& [set] = registry.ctx().get<renderer::RenderItemSet>();

		const auto view = registry
				.view<
					state::InCameraArea,
					render_effect::Position,
					render_effect::Scale,
					render_effect::Rotation,
					render_effect::Color
				>(entt::exclude<state::DisableRender>);

		for (const auto [entity, position, scale, rotation, color]: view.each())
		{
			auto it = set.find(entity);
			if (it == set.end())
			{
				SPDLOG_WARN(
					"实体(0x{:08x})的特效无法应用,因为其未参与渲染!",
					entt::to_integral(entity)
				);

				continue;
			}

			auto& item = it->second;

			item.position += position.extra;
			item.scale.x *= scale.extra.x;
			item.scale.y *= scale.extra.y;
			item.rotation += rotation.extra;
			item.color = color.color;
		}
	}
}
