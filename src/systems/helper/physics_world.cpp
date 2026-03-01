// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/physics_world.hpp>

#include <components/physics_world.hpp>

#include <game/constants.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::helper
{
	auto PhysicsWorld::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		auto def = b2DefaultWorldDef();
		// 无重力世界
		def.gravity = b2Vec2_zero;
		const auto id = b2CreateWorld(&def);

		registry.ctx().emplace<physics_world::Id>(id);
	}

	auto PhysicsWorld::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		const auto [id] = registry.ctx().get<physics_world::Id>();
		b2DestroyWorld(id);
	}

	auto PhysicsWorld::id(entt::registry& registry) noexcept -> b2WorldId
	{
		using namespace components;

		return registry.ctx().get<physics_world::Id>().id;
	}

	auto PhysicsWorld::to_user_data(const entt::entity entity) noexcept -> void*
	{
		return std::bit_cast<void*>(static_cast<std::uintptr_t>(entt::to_integral(entity)));
	}

	auto PhysicsWorld::to_entity(const void* user_data) noexcept -> entt::entity
	{
		return entt::entity{static_cast<std::underlying_type_t<entt::entity>>(std::bit_cast<std::uintptr_t>(user_data))};
	}

	auto PhysicsWorld::physics_position_of(const sf::Vector2f position) noexcept -> b2Vec2
	{
		return Constant::to_physics(position);
	}

	auto PhysicsWorld::position_of(const b2Vec2 physics_position) noexcept -> sf::Vector2f
	{
		return Constant::from_physics(physics_position);
	}

	auto PhysicsWorld::physics_size_of(const sf::Vector2f size) noexcept -> b2Vec2
	{
		return Constant::to_physics(size);
	}

	auto PhysicsWorld::size_of(const b2Vec2 physics_size) noexcept -> sf::Vector2f
	{
		return Constant::from_physics(physics_size);
	}

	auto PhysicsWorld::physics_rotation_of(const sf::Angle angle) noexcept -> b2Rot
	{
		return b2MakeRot(angle.asRadians());
	}

	auto PhysicsWorld::rotation_of(const b2Rot physics_angle) noexcept -> sf::Angle
	{
		const auto angle = b2Rot_GetAngle(physics_angle);

		return sf::radians(angle);
	}
}
