// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/transform.hpp>

#include <ranges>

#include <component/transform.hpp>

#include <helper/camera.hpp>

#include <entt/entt.hpp>

namespace pd::helper
{
	using namespace component;

	auto Transform::get_position(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2f
	{
		const auto* position = registry.try_get<const transform::Position>(entity);
		if (position == nullptr)
		{
			return {0, 0};
		}

		return position->position;
	}

	auto Transform::set_position(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_position) noexcept -> void
	{
		auto* position = registry.try_get<transform::Position>(entity);
		auto* screen_position = registry.try_get<transform::ScreenPosition>(entity);

		if (position == nullptr or screen_position == nullptr)
		{
			return;
		}

		const auto camera_position = Camera::get_position(registry);
		const auto relative = new_position - camera_position;

		position->position = new_position;
		screen_position->position = sf::Vector2i{relative};
	}

	auto Transform::set_position(entt::registry& registry, const std::span<const entt::entity> entities, const std::span<const sf::Vector2f> new_positions) noexcept -> void
	{
		const auto camera_position = Camera::get_position(registry);

		for (const auto [entity, new_position]: std::views::zip(entities, new_positions))
		{
			auto* position = registry.try_get<transform::Position>(entity);
			auto* screen_position = registry.try_get<transform::ScreenPosition>(entity);

			if (position == nullptr or screen_position == nullptr)
			{
				continue;
			}

			const auto relative = new_position - camera_position;

			position->position = new_position;
			screen_position->position = sf::Vector2i{relative};
		}
	}

	auto Transform::get_screen_position(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2i
	{
		const auto* screen_position = registry.try_get<const transform::ScreenPosition>(entity);
		if (screen_position == nullptr)
		{
			return {0, 0};
		}

		return screen_position->position;
	}

	auto Transform::set_screen_position(entt::registry& registry, const entt::entity entity, const sf::Vector2i new_position) noexcept -> void
	{
		auto* position = registry.try_get<transform::Position>(entity);
		auto* screen_position = registry.try_get<transform::ScreenPosition>(entity);

		if (position == nullptr or screen_position == nullptr)
		{
			return;
		}

		const auto camera_position = Camera::get_position(registry);
		const auto relative = new_position + sf::Vector2i{camera_position};

		position->position = sf::Vector2f{relative};
		screen_position->position = new_position;
	}

	auto Transform::set_screen_position(entt::registry& registry, const std::span<const entt::entity> entities, const std::span<const sf::Vector2i> new_positions) noexcept -> void
	{
		const auto camera_position = Camera::get_position(registry);

		for (const auto [entity, new_position]: std::views::zip(entities, new_positions))
		{
			auto* position = registry.try_get<transform::Position>(entity);
			auto* screen_position = registry.try_get<transform::ScreenPosition>(entity);

			if (position == nullptr or screen_position == nullptr)
			{
				return;
			}

			const auto relative = new_position + sf::Vector2i{camera_position};

			position->position = sf::Vector2f{relative};
			screen_position->position = new_position;
		}
	}

	auto Transform::translate(entt::registry& registry, const entt::entity entity, const sf::Vector2f distance) noexcept -> void
	{
		auto* position = registry.try_get<transform::Position>(entity);
		auto* screen_position = registry.try_get<transform::ScreenPosition>(entity);

		if (position == nullptr or screen_position == nullptr)
		{
			return;
		}

		position->position += distance;
		screen_position->position += sf::Vector2i{distance};
	}

	auto Transform::get_rotation(entt::registry& registry, const entt::entity entity) noexcept -> sf::Angle
	{
		const auto* rotation = registry.try_get<const transform::Rotation>(entity);
		if (rotation == nullptr)
		{
			return sf::degrees(0);
		}

		return rotation->rotation;
	}

	auto Transform::set_rotation(entt::registry& registry, const entt::entity entity, const sf::Angle new_rotation) noexcept -> void
	{
		auto* rotation = registry.try_get<transform::Rotation>(entity);
		if (rotation == nullptr)
		{
			return;
		}

		rotation->rotation = new_rotation;
	}
}
