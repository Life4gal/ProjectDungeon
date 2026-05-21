// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/physics.hpp>

#include <ranges>

#include <utility/physics.hpp>

#include <component/physics.hpp>

#include <helper/camera.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::helper
{
	using namespace component;

	auto Physics::disable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Disable(body_id->body_id);
	}

	auto Physics::enable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Enable(body_id->body_id);
	}

	auto Physics::get_position(entt::registry& registry, const entt::entity entity) noexcept -> b2Vec2
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Vec2_zero;
		}

		return b2Body_GetPosition(body_id->body_id);
	}

	auto Physics::set_position(entt::registry& registry, const entt::entity entity, const b2Vec2 new_position) noexcept -> void
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto rotation = b2Body_GetRotation(body_id->body_id);
		b2Body_SetTransform(body_id->body_id, new_position, rotation);
	}

	auto Physics::get_pixel_position(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2f
	{
		const auto position = get_position(registry, entity);
		if (position == b2Vec2_zero)
		{
			return {0, 0};
		}

		return utility::Physics::from_physics(position);
	}

	auto Physics::set_pixel_position(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_position) noexcept -> void
	{
		const auto new_physics_position = utility::Physics::to_physics(new_position);

		set_position(registry, entity, new_physics_position);
	}

	auto Physics::get_screen_position(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2i
	{
		const auto pixel_position = get_pixel_position(registry, entity);
		// get_pixel_position返回{0,0}其实有两种可能:
		//  1.不存在物理刚体组件
		//  2.实体的确位于{0,0}
		// 此时返回{0,0}理论上无论如何都不算错?
		if (pixel_position == sf::Vector2f{0, 0})
		{
			return {0, 0};
		}

		const auto camera_position = Camera::get_position(registry);
		const auto relative = pixel_position - camera_position;

		return sf::Vector2i{relative};
	}

	auto Physics::get_screen_position(entt::registry& registry, const std::span<const entt::entity> entities) noexcept -> std::vector<sf::Vector2i>
	{
		const auto camera_position = Camera::get_position(registry);

		std::vector<sf::Vector2i> screen_positions{};
		screen_positions.resize(entities.size());

		for (auto [entity, screen_position]: std::views::zip(entities, screen_positions))
		{
			const auto pixel_position = get_pixel_position(registry, entity);
			// @see get_screen_position
			if (pixel_position == sf::Vector2f{0, 0})
			{
				screen_position = {0, 0};
				continue;
			}

			const auto relative = pixel_position - camera_position;

			screen_position = sf::Vector2i{relative};
		}

		return screen_positions;
	}

	auto Physics::set_screen_position(entt::registry& registry, const entt::entity entity, const sf::Vector2i new_position) noexcept -> void
	{
		const auto camera_position = Camera::get_position(registry);
		const auto pixel_position = sf::Vector2i{camera_position} + new_position;

		set_pixel_position(registry, entity, sf::Vector2f{pixel_position});
	}

	auto Physics::set_screen_position(entt::registry& registry, const std::span<const entt::entity> entities, const std::span<const sf::Vector2i> new_positions) noexcept -> void
	{
		const auto camera_position = Camera::get_position(registry);

		for (const auto [entity, new_position]: std::views::zip(entities, new_positions))
		{
			const auto pixel_position = sf::Vector2i{camera_position} + new_position;

			set_pixel_position(registry, entity, sf::Vector2f{pixel_position});
		}
	}

	auto Physics::translate(entt::registry& registry, const entt::entity entity, const b2Vec2 distance) noexcept -> void
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		auto [position, rotation] = b2Body_GetTransform(body_id->body_id);
		position += distance;
		b2Body_SetTransform(body_id->body_id, position, rotation);
	}

	auto Physics::translate_pixel(entt::registry& registry, const entt::entity entity, const sf::Vector2f distance) noexcept -> void
	{
		const auto physics_distance = utility::Physics::to_physics(distance);

		translate(registry, entity, physics_distance);
	}

	auto Physics::get_rotation(entt::registry& registry, const entt::entity entity) noexcept -> b2Rot
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Rot_identity;
		}

		return b2Body_GetRotation(body_id->body_id);
	}

	auto Physics::set_rotation(entt::registry& registry, const entt::entity entity, const b2Rot new_rotation) noexcept -> void
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto position = b2Body_GetPosition(body_id->body_id);
		b2Body_SetTransform(body_id->body_id, position, new_rotation);
	}

	auto Physics::get_transform(entt::registry& registry, const entt::entity entity) noexcept -> b2Transform
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Transform_identity;
		}

		return b2Body_GetTransform(body_id->body_id);
	}

	auto Physics::set_transform(entt::registry& registry, const entt::entity entity, const b2Transform new_transform) noexcept -> void
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_SetTransform(body_id->body_id, new_transform.p, new_transform.q);
	}


	auto Physics::get_linear_velocity(entt::registry& registry, const entt::entity entity) noexcept -> b2Vec2
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Vec2_zero;
		}

		return get_linear_velocity(body_id->body_id);
	}

	auto Physics::get_linear_velocity(const b2BodyId body_id) noexcept -> b2Vec2
	{
		return b2Body_GetLinearVelocity(body_id);
	}

	auto Physics::set_linear_velocity(entt::registry& registry, const entt::entity entity, const b2Vec2 new_velocity) noexcept -> void
	{
		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		set_linear_velocity(body_id->body_id, new_velocity);
	}

	auto Physics::set_linear_velocity(const b2BodyId body_id, const b2Vec2 new_velocity) noexcept -> void
	{
		b2Body_SetLinearVelocity(body_id, new_velocity);
	}
}
