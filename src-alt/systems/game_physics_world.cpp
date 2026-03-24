// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/game_physics_world.hpp>

#include <bit>

#include <components/game_physics_world.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems
{
	namespace
	{
		constexpr auto pixels_per_meter = 32.f;
		constexpr auto meters_per_pixel = 1.f / pixels_per_meter;

		[[nodiscard]] /*constexpr*/ auto from_physics(const b2Vec2 physics) noexcept -> sf::Vector2f
		{
			const auto p = pixels_per_meter * physics;
			return {p.x, p.y};
		}

		[[nodiscard]] /*constexpr*/ auto to_physics(const sf::Vector2f pixels) noexcept -> b2Vec2
		{
			const auto p = meters_per_pixel * pixels;
			return {p.x, p.y};
		}
	}

	auto GamePhysicsWorld::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		auto def = b2DefaultWorldDef();
		// 无重力世界(俯视角)
		def.gravity = b2Vec2_zero;
		const auto id = b2CreateWorld(&def);

		registry.ctx().emplace<game_physics_world::Id>(id);
	}

	auto GamePhysicsWorld::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		const auto [id] = registry.ctx().get<game_physics_world::Id>();
		b2DestroyWorld(id);

		registry.ctx().erase<game_physics_world::Id>();
	}

	auto GamePhysicsWorld::id(entt::registry& registry) noexcept -> b2WorldId
	{
		using namespace components;

		const auto [id] = registry.ctx().get<const game_physics_world::Id>();

		return id;
	}

	auto GamePhysicsWorld::to_user_data(const entt::entity entity) noexcept -> void*
	{
		return std::bit_cast<void*>(static_cast<std::uintptr_t>(entt::to_integral(entity)));
	}

	auto GamePhysicsWorld::to_entity(const void* user_data) noexcept -> entt::entity
	{
		return entt::entity{static_cast<std::underlying_type_t<entt::entity>>(std::bit_cast<std::uintptr_t>(user_data))};
	}

	auto GamePhysicsWorld::physics_position_of(const sf::Vector2f position) noexcept -> b2Vec2
	{
		return to_physics(position);
	}

	auto GamePhysicsWorld::position_of(const b2Vec2 physics_position) noexcept -> sf::Vector2f
	{
		return from_physics(physics_position);
	}

	auto GamePhysicsWorld::physics_size_of(const sf::Vector2f size) noexcept -> b2Vec2
	{
		return to_physics(size);
	}

	auto GamePhysicsWorld::size_of(const b2Vec2 physics_size) noexcept -> sf::Vector2f
	{
		return from_physics(physics_size);
	}

	auto GamePhysicsWorld::physics_rotation_of(const sf::Angle angle) noexcept -> b2Rot
	{
		return b2MakeRot(angle.asRadians());
	}

	auto GamePhysicsWorld::rotation_of(const b2Rot physics_angle) noexcept -> sf::Angle
	{
		const auto angle = b2Rot_GetAngle(physics_angle);

		return sf::radians(angle);
	}
}
