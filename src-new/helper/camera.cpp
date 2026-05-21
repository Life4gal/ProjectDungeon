// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/camera.hpp>

#include <manager/event.hpp>

#include <event/camera.hpp>

#include <component/camera.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	namespace
	{
		auto do_set_position(entt::registry& registry, const sf::Vector2f new_position) noexcept -> sf::Vector2f
		{
			auto& [position] = registry.ctx().get<camera::Position>();

			return std::exchange(position, new_position);
		}

		auto do_set_size(entt::registry& registry, const sf::Vector2f new_size) noexcept -> sf::Vector2f
		{
			auto& [size] = registry.ctx().get<camera::Size>();

			return std::exchange(size, new_size);
		}
	}

	auto Camera::dirty(entt::registry& registry) noexcept -> bool
	{
		return registry.ctx().contains<camera::Dirty>();
	}

	auto Camera::clean(entt::registry& registry) noexcept -> void
	{
		registry.ctx().erase<camera::Dirty>();
	}

	auto Camera::initialize(entt::registry& registry, const sf::FloatRect area) noexcept -> void
	{
		registry.ctx().emplace<camera::Dirty>();

		registry.ctx().insert_or_assign(camera::Position{.position = area.position});
		registry.ctx().insert_or_assign(camera::Size{.size = area.size});
	}

	auto Camera::get_area(entt::registry& registry) noexcept -> sf::FloatRect
	{
		const auto position = get_position(registry);
		const auto size = get_size(registry);

		return {position, size};
	}

	auto Camera::set_area(entt::registry& registry, const sf::FloatRect new_area) noexcept -> void
	{
		registry.ctx().emplace<camera::Dirty>();

		[[maybe_unused]] const auto old_position = do_set_position(registry, new_area.position);
		[[maybe_unused]] const auto old_size = do_set_size(registry, new_area.size);

		SPDLOG_INFO(
			"设置相机区域: [X]={}, [Y]={}, [Width]={}, [Height]={}",
			new_area.position.x,
			new_area.position.y,
			new_area.position.x,
			new_area.position.y
		);

		manager::Event::enqueue(event::camera::SetArea{.previous = {old_position, old_size}, .current = new_area});
	}

	auto Camera::get_position(entt::registry& registry) noexcept -> sf::Vector2f
	{
		const auto& [position] = registry.ctx().get<const camera::Position>();

		return position;
	}

	auto Camera::set_position(entt::registry& registry, const sf::Vector2f new_position) noexcept -> void
	{
		registry.ctx().emplace<camera::Dirty>();

		const auto old_position = do_set_position(registry, new_position);

		SPDLOG_INFO(
			"相机位置移动: [X]={} -> {}({}), [Y]={} -> {}({})",
			old_position.x,
			new_position.x,
			new_position.x - old_position.x,
			old_position.y,
			new_position.y,
			new_position.y - old_position.y
		);

		manager::Event::enqueue(event::camera::Move{.previous = old_position, .current = new_position});
	}

	auto Camera::translate(entt::registry& registry, const sf::Vector2f distance) noexcept -> void
	{
		registry.ctx().emplace<camera::Dirty>();

		const auto old_position = get_position(registry);
		const auto new_position = old_position + distance;
		[[maybe_unused]] const auto should_be_old_position = do_set_position(registry, new_position);

		SPDLOG_INFO(
			"相机位置平移: [X]={} -> {}({}), [Y]={} -> {}({})",
			old_position.x,
			new_position.x,
			new_position.x - old_position.x,
			old_position.y,
			new_position.y,
			new_position.y - old_position.y
		);

		manager::Event::enqueue(event::camera::Translate{.previous = old_position, .current = new_position, .distance = distance});
	}

	auto Camera::get_size(entt::registry& registry) noexcept -> sf::Vector2f
	{
		const auto& [size] = registry.ctx().get<const camera::Size>();

		return size;
	}

	auto Camera::set_size(entt::registry& registry, const sf::Vector2f new_size) noexcept -> void
	{
		registry.ctx().emplace<camera::Dirty>();

		const auto old_size = do_set_size(registry, new_size);

		SPDLOG_INFO(
			"设置相机区域大小: [Width]={} -> {}({}), [Height]={} -> {}({})",
			old_size.x,
			new_size.x,
			new_size.x - old_size.x,
			old_size.y,
			new_size.y,
			new_size.y - old_size.y
		);

		manager::Event::enqueue(event::camera::Resize{.previous = old_size, .current = new_size});
	}
}
