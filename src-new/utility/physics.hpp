// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

// std::bit_cast
#include <bit>

#include <entt/entity/entity.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>
#include <box2d/id.h>
#include <box2d/math_functions.h>

namespace pd::utility
{
	// TODO: 这个类不应该定义在这里,不过方便起见,先暂时这么做
	class Physics final
	{
	public:
		constexpr static auto pixels_per_meter = 32.f;
		constexpr static auto meters_per_pixel = 1.f / pixels_per_meter;

		inline static b2WorldId world_id = b2_nullWorldId;

	private:
		[[nodiscard]] static auto from_physics(const b2Vec2 value) noexcept -> sf::Vector2f
		{
			const auto [x, y] = pixels_per_meter * value;
			return {x, y};
		}

		[[nodiscard]] static auto to_physics(const sf::Vector2f value) noexcept -> b2Vec2
		{
			const auto [x, y] = meters_per_pixel * value;
			return {.x = x, .y = y};
		}

	public:
		[[nodiscard]] static auto to_user_data(const entt::entity entity) noexcept -> void*
		{
			return std::bit_cast<void*>(static_cast<std::uintptr_t>(entt::to_integral(entity)));
		}

		[[nodiscard]] static auto to_entity(const void* user_data) noexcept -> entt::entity
		{
			return entt::entity{static_cast<std::underlying_type_t<entt::entity>>(std::bit_cast<std::uintptr_t>(user_data))};
		}

		[[nodiscard]] static auto to_physics_position(const sf::Vector2f position) noexcept -> b2Vec2
		{
			return to_physics(position);
		}

		[[nodiscard]] static auto from_physics_position(const b2Vec2 position) noexcept -> sf::Vector2f
		{
			return from_physics(position);
		}

		[[nodiscard]] static auto to_physics_size(const sf::Vector2f size) noexcept -> b2Vec2
		{
			return to_physics(size);
		}

		[[nodiscard]] static auto from_physics_size(const b2Vec2 size) noexcept -> sf::Vector2f
		{
			return from_physics(size);
		}

		[[nodiscard]] static auto to_physics_rotation(const sf::Angle angle) noexcept -> b2Rot
		{
			return b2MakeRot(angle.asRadians());
		}

		[[nodiscard]] static auto from_physics_rotation(const b2Rot rotation) noexcept -> sf::Angle
		{
			return sf::radians(b2Rot_GetAngle(rotation));
		}
	};
}
