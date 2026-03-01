// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/render/debug.hpp>

#include <algorithm>

#include <config/animation.hpp>

#include <components/transform.hpp>
#include <components/render.hpp>
#include <components/physics_body.hpp>

#include <systems/helper/physics_world.hpp>

#include <entt/entt.hpp>
#include <imgui.h>

namespace pd::systems::render
{
	auto debug(entt::registry& registry, [[maybe_unused]] sf::RenderWindow& window) noexcept -> void
	{
		using namespace components;

		auto* draw_list = ImGui::GetBackgroundDrawList();

		const auto view = registry.view<
			const components::render::AnimationFrame,
			transform::Position,
			transform::Scale
		>(entt::exclude<components::render::Invisible>);

		for (const auto [entity, animation_frame, center_position, scale]: view.each())
		{
			const auto& frame = animation_frame.animation_frame.get();
			const auto texture_width = static_cast<float>(frame.texture_width);
			const auto texture_height = static_cast<float>(frame.texture_height);

			const auto width = texture_width * scale.scale.x;
			const auto height = texture_height * scale.scale.y;

			const auto size = sf::Vector2f{width, height};
			const auto position = center_position.position - size / 2.f;

			draw_list->AddRect(ImVec2{position.x, position.y}, ImVec2{position.x + size.x, position.y + size.y},IM_COL32(255, 0, 0, 255));

			std::string info{};
			info.reserve(48);

			std::format_to(
				std::back_inserter(info),
				"Entity: {}\n({:.1f}.{:.1f})",
				entt::to_integral(entity),
				position.x,
				position.y
			);

			if (registry.all_of<physics_body::BodyId>(entity))
			{
				const auto physics_position = helper::PhysicsWorld::physics_position_of(position);
				std::format_to(
					std::back_inserter(info),
					"\n({:.1f}.{:.1f})",
					physics_position.x,
					physics_position.y
				);
			}

			draw_list->AddText(ImVec2{position.x, position.y}, IM_COL32(0, 255, 0, 255), info.c_str(), info.c_str() + info.size());
		}
	}
}
