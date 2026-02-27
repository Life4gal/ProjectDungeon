// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility>
#include <type_traits>

#include <config/render_layer.hpp>

namespace pd::config
{
	enum class CollisionMask : std::underlying_type_t<RenderLayer>
	{
		WALL = //
		std::to_underlying(RenderLayer::ENEMY_GROUND) |
		std::to_underlying(RenderLayer::PLAYER_GROUND) |
		std::to_underlying(RenderLayer::ENEMY_AERIAL) |
		std::to_underlying(RenderLayer::PLAYER_AERIAL) |
		std::to_underlying(RenderLayer::PROJECTILE) |
		std::to_underlying(RenderLayer::EFFECT),

		TRIGGER = //
		std::to_underlying(RenderLayer::ENEMY_GROUND) |
		std::to_underlying(RenderLayer::PLAYER_GROUND) |
		std::to_underlying(RenderLayer::ENEMY_AERIAL) |
		std::to_underlying(RenderLayer::PLAYER_AERIAL),

		DOOR_CLOSE = //
		WALL,

		DOOR_OPEN = //
		std::to_underlying(RenderLayer::PLAYER_GROUND),

		ENEMY_GROUND = //
		std::to_underlying(RenderLayer::ENEMY_GROUND) |
		std::to_underlying(RenderLayer::PLAYER_GROUND) |
		std::to_underlying(RenderLayer::PROJECTILE) |
		std::to_underlying(RenderLayer::EFFECT),

		PLAYER_GROUND = //
		std::to_underlying(RenderLayer::ENEMY_GROUND) |
		std::to_underlying(RenderLayer::PLAYER_GROUND) |
		std::to_underlying(RenderLayer::PROJECTILE) |
		std::to_underlying(RenderLayer::EFFECT),

		ENEMY_AERIAL = //
		std::to_underlying(RenderLayer::ENEMY_AERIAL) |
		std::to_underlying(RenderLayer::PLAYER_AERIAL) |
		std::to_underlying(RenderLayer::PROJECTILE) |
		std::to_underlying(RenderLayer::EFFECT),

		PLAYER_AERIAL = //
		std::to_underlying(RenderLayer::ENEMY_AERIAL) |
		std::to_underlying(RenderLayer::PLAYER_AERIAL) |
		std::to_underlying(RenderLayer::PROJECTILE) |
		std::to_underlying(RenderLayer::EFFECT),

		PROJECTILE = //
		std::to_underlying(RenderLayer::WALL) |
		std::to_underlying(RenderLayer::ENEMY_GROUND) |
		std::to_underlying(RenderLayer::PLAYER_GROUND) |
		std::to_underlying(RenderLayer::ENEMY_AERIAL) |
		std::to_underlying(RenderLayer::PLAYER_AERIAL),

		EFFECT = //
		std::to_underlying(RenderLayer::WALL) |
		std::to_underlying(RenderLayer::ENEMY_GROUND) |
		std::to_underlying(RenderLayer::PLAYER_GROUND) |
		std::to_underlying(RenderLayer::ENEMY_AERIAL) |
		std::to_underlying(RenderLayer::PLAYER_AERIAL),
	};
}
