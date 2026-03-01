// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/render/debug.hpp>

#include <algorithm>

#include <config/animation.hpp>

#include <components/name.hpp>
#include <components/transform.hpp>
#include <components/render.hpp>
#include <components/physics_body.hpp>

#include <systems/helper/physics_body.hpp>

#include <entt/entt.hpp>
#include <imgui.h>

namespace pd::systems::render
{
	auto debug(entt::registry& registry, [[maybe_unused]] sf::RenderWindow& window) noexcept -> void
	{
		using namespace components;

		const auto mouse_position = ImGui::GetMousePos();
		auto* draw_list = ImGui::GetBackgroundDrawList();
		draw_list->AddRect(
			{mouse_position.x - 5, mouse_position.y - 5},
			ImVec2{mouse_position.x + 5, mouse_position.y + 5},
			IM_COL32(0, 0, 255, 255)
		);

		const auto view = registry.view<
			const name::Name,
			const components::render::AnimationFrame,
			const transform::Position,
			const transform::Scale>(entt::exclude<components::render::Invisible>);

		for (const auto [entity, name, animation_frame, center_position, scale]: view.each())
		{
			const auto& frame = animation_frame.animation_frame.get();
			const auto texture_width = static_cast<float>(frame.texture_width);
			const auto texture_height = static_cast<float>(frame.texture_height);

			const auto width = texture_width * scale.scale.x;
			const auto height = texture_height * scale.scale.y;

			const auto size = sf::Vector2f{width, height};
			const auto position = center_position.position - size / 2.f;

			if (
				mouse_position.x >= position.x and
				mouse_position.x <= position.x + size.x and
				mouse_position.y >= position.y and
				mouse_position.y <= position.y + size.y
			)
			{
				draw_list->AddRect(
					ImVec2{position.x, position.y},
					ImVec2{position.x + size.x, position.y + size.y},
					IM_COL32(255, 0, 0, 255)
				);

				std::string info{};
				info.reserve(48);

				std::format_to(std::back_inserter(info), "{}:\nPosition: ({:.1f}.{:.1f})", name.name, position.x, position.y);

				if (registry.all_of<physics_body::BodyId>(entity))
				{
					const auto physics_position = helper::PhysicsBody::get_position(registry, entity);
					std::format_to(std::back_inserter(info), "\nPhysics Position: ({:.1f}.{:.1f})", physics_position.x, physics_position.y);
				}

				const auto text_size = ImGui::CalcTextSize(info.c_str());
				const auto tooltip_pos = ImVec2(mouse_position.x + 10, mouse_position.y - 10 - text_size.y);

				draw_list->AddRectFilled(
					ImVec2{tooltip_pos.x - 5, tooltip_pos.y - 5},
					ImVec2{tooltip_pos.x + text_size.x + 5, tooltip_pos.y + text_size.y + 5},
					IM_COL32(30, 30, 30, 230),
					4.0f
				);

				draw_list->AddText(tooltip_pos, IM_COL32(255, 0, 0, 255), info.c_str());
			}
			else
			{
				draw_list->AddText(
					ImVec2{position.x, position.y},
					IM_COL32(0, 255, 0, 255),
					name.name.c_str(),
					name.name.c_str() + name.name.size()
				);
			}
		}
	}
}
