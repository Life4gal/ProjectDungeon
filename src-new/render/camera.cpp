// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/camera.hpp>

#include <component/camera.hpp>
#include <component/state.hpp>
#include <component/position.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	auto camera(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		if (not registry.ctx().contains<camera::Dirty>())
		{
			return;
		}

		const auto& [camera_position] = registry.ctx().get<const camera::Position>();
		const auto& [camera_size] = registry.ctx().get<const camera::Size>();
		const auto camera_area = sf::Rect{camera_position, camera_size};

		// 设置窗口视图
		window.setView(sf::View{camera_area});

		// 更新可视实体
		for (const auto view = registry.view<position::World>();
		     const auto [entity, position]: view.each())
		{
			// TODO: 不能简单地用position筛选处于相机范围内的实体,还需要考虑实体的size,当且仅当实体的bounding box完全不与相机范围相交时才认为实体不在相机范围内

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
