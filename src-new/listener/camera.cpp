// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/camera.hpp>

#include <component/camera.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::listener::camera
{
	auto on_set(entt::registry& registry, const event::camera::Set& set) noexcept -> void
	{
		namespace camera = component::camera;

		registry.ctx().emplace<camera::Dirty>();

		registry.ctx().insert_or_assign(camera::Position{.position = {set.x, set.y}});
		registry.ctx().insert_or_assign(camera::Size{.size = {set.width, set.height}});

		SPDLOG_INFO("Camera Set: [X]={}, [Y]={}, [Width]={}, [Height]={}", set.x, set.y, set.width, set.height);
	}

	auto on_move_to(entt::registry& registry, const event::camera::MoveTo& move_to) noexcept -> void
	{
		namespace camera = component::camera;

		registry.ctx().emplace<camera::Dirty>();

		auto& [position] = registry.ctx().get<camera::Position>();
		const sf::Vector2f new_position{move_to.x, move_to.y};

		SPDLOG_INFO("Camera MoveTo: [X]={} -> {}, [Y]={} -> {}", position.x, new_position.x, position.y, new_position.y);

		position = new_position;
	}

	auto on_translate(entt::registry& registry, const event::camera::Translate& translate) noexcept -> void
	{
		namespace camera = component::camera;

		registry.ctx().emplace<camera::Dirty>();

		auto& [position] = registry.ctx().get<camera::Position>();
		const sf::Vector2f new_position{position.x + translate.x, position.y + translate.y};

		SPDLOG_INFO("Camera Translate: [X]={} -> {}, [Y]={} -> {}", position.x, new_position.x, position.y, new_position.y);

		position = new_position;
	}

	auto on_resize(entt::registry& registry, const event::camera::Resize& resize) noexcept -> void
	{
		namespace camera = component::camera;

		registry.ctx().emplace<camera::Dirty>();

		auto& [size] = registry.ctx().get<camera::Size>();
		const sf::Vector2f new_size{resize.width, resize.height};

		SPDLOG_INFO("Camera Resize: [Width]={} -> {}, [Height]={} -> {}", size.x, new_size.x, size.y, new_size.y);

		size = new_size;
	}
}
