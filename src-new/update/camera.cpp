// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/camera.hpp>

#include <component/camera.hpp>
#include <component/state.hpp>
#include <component/transform.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace pd::update
{
	using namespace component;

	auto camera(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		if (not registry.ctx().contains<camera::Dirty>())
		{
			return;
		}

		const auto& [camera_position] = registry.ctx().get<const camera::Position>();
		const auto& [camera_size] = registry.ctx().get<const camera::Size>();
		const auto camera_area = sf::Rect{camera_position, camera_size};

		for (const auto view = registry.view<transform::Position>();
		     const auto [entity, position]: view.each())
		{
			if (camera_area.contains(position.position))
			{
				registry.emplace_or_replace<state::InCameraArea>(entity);
			}
			else
			{
				registry.remove<state::InCameraArea>(entity);
			}
		}
	}
}
