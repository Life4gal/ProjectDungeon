// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <accessor/collision.hpp>

#include <utility/physics.hpp>

#include <component/collision.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::accessor
{
	using namespace component;

	auto Collision::disable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Disable(body_id->body_id);
	}

	auto Collision::enable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Enable(body_id->body_id);
	}

	auto Collision::get_position(entt::registry& registry, const entt::entity entity) noexcept -> b2Vec2
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Vec2_zero;
		}

		return b2Body_GetPosition(body_id->body_id);
	}

	auto Collision::set_position(entt::registry& registry, const entt::entity entity, const b2Vec2 new_position) noexcept -> void
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto rotation = b2Body_GetRotation(body_id->body_id);
		b2Body_SetTransform(body_id->body_id, new_position, rotation);
	}

	auto Collision::get_pixel_position(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2f
	{
		const auto position = get_position(registry, entity);
		if (position == b2Vec2_zero)
		{
			return {0, 0};
		}

		return utility::Physics::from_physics(position);
	}

	auto Collision::set_pixel_position(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_position) noexcept -> void
	{
		const auto new_physics_position = utility::Physics::to_physics(new_position);

		set_position(registry, entity, new_physics_position);
	}

	auto Collision::translate(entt::registry& registry, const entt::entity entity, const b2Vec2 distance) noexcept -> void
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		auto [position, rotation] = b2Body_GetTransform(body_id->body_id);
		position += distance;
		b2Body_SetTransform(body_id->body_id, position, rotation);
	}

	auto Collision::translate_pixel(entt::registry& registry, const entt::entity entity, const sf::Vector2f distance) noexcept -> void
	{
		const auto physics_distance = utility::Physics::to_physics(distance);

		translate(registry, entity, physics_distance);
	}

	auto Collision::get_rotation(entt::registry& registry, const entt::entity entity) noexcept -> b2Rot
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Rot_identity;
		}

		return b2Body_GetRotation(body_id->body_id);
	}

	auto Collision::set_rotation(entt::registry& registry, const entt::entity entity, const b2Rot new_rotation) noexcept -> void
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto position = b2Body_GetPosition(body_id->body_id);
		b2Body_SetTransform(body_id->body_id, position, new_rotation);
	}

	auto Collision::get_transform(entt::registry& registry, const entt::entity entity) noexcept -> b2Transform
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Transform_identity;
		}

		return b2Body_GetTransform(body_id->body_id);
	}

	auto Collision::set_transform(entt::registry& registry, const entt::entity entity, const b2Transform new_transform) noexcept -> void
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_SetTransform(body_id->body_id, new_transform.p, new_transform.q);
	}


	auto Collision::get_linear_velocity(entt::registry& registry, const entt::entity entity) noexcept -> b2Vec2
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Vec2_zero;
		}

		return get_linear_velocity(body_id->body_id);
	}

	auto Collision::get_linear_velocity(const b2BodyId body_id) noexcept -> b2Vec2
	{
		return b2Body_GetLinearVelocity(body_id);
	}

	auto Collision::set_linear_velocity(entt::registry& registry, const entt::entity entity, const b2Vec2 new_velocity) noexcept -> void
	{
		const auto* body_id = registry.try_get<const collision::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		set_linear_velocity(body_id->body_id, new_velocity);
	}

	auto Collision::set_linear_velocity(const b2BodyId body_id, const b2Vec2 new_velocity) noexcept -> void
	{
		b2Body_SetLinearVelocity(body_id, new_velocity);
	}
}
