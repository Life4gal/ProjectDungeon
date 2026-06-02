// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/camera.hpp>

#include <component/camera.hpp>

#include <helper/camera.hpp>

#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	auto camera(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		if (not helper::Camera::dirty(registry))
		{
			return;
		}
		helper::Camera::clean(registry);

		const auto camera_area = helper::Camera::get_area(registry);

		// 设置窗口视图
		window.setView(sf::View{camera_area});
	}
}
