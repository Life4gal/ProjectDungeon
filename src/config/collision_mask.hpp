// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility>

#include <config/render_layer.hpp>

namespace pd::config
{
	class CollisionMask final
	{
	public:
		constexpr static auto wall = //
				std::to_underlying(RenderLayer::ENEMY_GROUND) |
				std::to_underlying(RenderLayer::PLAYER_GROUND) |
				std::to_underlying(RenderLayer::ENEMY_AERIAL) |
				std::to_underlying(RenderLayer::PLAYER_AERIAL) |
				std::to_underlying(RenderLayer::PROJECTILE) |
				std::to_underlying(RenderLayer::EFFECT);

		constexpr static auto trigger = //
				std::to_underlying(RenderLayer::ENEMY_GROUND) |
				std::to_underlying(RenderLayer::PLAYER_GROUND) |
				std::to_underlying(RenderLayer::ENEMY_AERIAL) |
				std::to_underlying(RenderLayer::PLAYER_AERIAL);

		constexpr static auto door_close = //
				wall;

		constexpr static auto door_open = //
				std::to_underlying(RenderLayer::PLAYER_GROUND);

		constexpr static auto enemy_ground = //
				std::to_underlying(RenderLayer::WALL) |
				std::to_underlying(RenderLayer::ENEMY_GROUND) |
				std::to_underlying(RenderLayer::PLAYER_GROUND) |
				std::to_underlying(RenderLayer::PROJECTILE) |
				std::to_underlying(RenderLayer::EFFECT);

		constexpr static auto player_ground = //
				std::to_underlying(RenderLayer::WALL) |
				std::to_underlying(RenderLayer::DOOR) |
				std::to_underlying(RenderLayer::ENEMY_GROUND) |
				std::to_underlying(RenderLayer::PLAYER_GROUND) |
				std::to_underlying(RenderLayer::PROJECTILE) |
				std::to_underlying(RenderLayer::EFFECT);

		constexpr static auto enemy_aerial = //
				std::to_underlying(RenderLayer::WALL) |
				std::to_underlying(RenderLayer::ENEMY_AERIAL) |
				std::to_underlying(RenderLayer::PLAYER_AERIAL) |
				std::to_underlying(RenderLayer::PROJECTILE) |
				std::to_underlying(RenderLayer::EFFECT);

		constexpr static auto player_aerial = //
				std::to_underlying(RenderLayer::WALL) |
				std::to_underlying(RenderLayer::ENEMY_AERIAL) |
				std::to_underlying(RenderLayer::PLAYER_AERIAL) |
				std::to_underlying(RenderLayer::PROJECTILE) |
				std::to_underlying(RenderLayer::EFFECT);

		constexpr static auto projectile = //
				std::to_underlying(RenderLayer::WALL) |
				std::to_underlying(RenderLayer::ENEMY_GROUND) |
				std::to_underlying(RenderLayer::PLAYER_GROUND) |
				std::to_underlying(RenderLayer::ENEMY_AERIAL) |
				std::to_underlying(RenderLayer::PLAYER_AERIAL);

		constexpr static auto effect = //
				std::to_underlying(RenderLayer::WALL) |
				std::to_underlying(RenderLayer::ENEMY_GROUND) |
				std::to_underlying(RenderLayer::PLAYER_GROUND) |
				std::to_underlying(RenderLayer::ENEMY_AERIAL) |
				std::to_underlying(RenderLayer::PLAYER_AERIAL);
	};
}
