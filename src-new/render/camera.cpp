// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/camera.hpp>

#include <component/camera.hpp>
#include <component/tags.hpp>
#include <component/transform.hpp>

#include <helper/camera.hpp>

#include <entt/entt.hpp>
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

		// 更新可视实体
		// for (const auto view = registry.view<state::CameraAware, transform::Position>();
		for (const auto view = registry.view<transform::Position>(entt::exclude<state::CameraIgnore>);
		     const auto [entity, position]: view.each())
		{
			// TODO: 不能简单地用position筛选处于相机范围内的实体,还需要考虑实体的size,当且仅当实体的bounding box完全不与相机范围相交时才认为实体不在相机范围内
			// ReSharper disable once CppTooWideScope
			const auto in_area = camera_area.contains(position.position);

			// TODO: 我们将InCameraArea拆分为多个子Awake本意是为了降低该标签威力
			//  同时允许之后进行更精细化的扩展,但是相对的,我们不得不将这些标签全部手动添加
			//  这意味着这里很容易出错(例如遗漏)
			if (in_area)
			{
				// SPRITE
				registry.emplace_or_replace<state::sprite::Awake>(entity);
				// PROPERTY
				registry.emplace_or_replace<state::property::Awake>(entity);
				// AI
				registry.emplace_or_replace<state::ai::Awake>(entity);
			}
			else
			{
				// SPRITE
				registry.remove<state::sprite::Awake>(entity);
				// PROPERTY
				registry.remove<state::property::Awake>(entity);
				// AI
				registry.remove<state::ai::Awake>(entity);
			}
		}
	}
}
