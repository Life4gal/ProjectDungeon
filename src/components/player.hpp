// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <vector>

#include <entt/entity/fwd.hpp>

namespace pd::components::player
{
	// ==========================================
	// 上下文组件
	// ==========================================

	// 当前玩家实体
	// 用于在其他系统中访问玩家
	class Player final
	{
	public:
		entt::entity entity;
	};

	// ==========================================
	// 实体组件
	// ==========================================

	// transform
	// render
	// animation
	// physics_body
	// actor

	class Movement final
	{
	public:
		enum class Type : std::uint8_t
		{
			STAY,
			FORWARD,
			BACKWARD,
		};

		Type x;
		Type y;
	};

	// 玩家当前拥有的钥匙列表
	class Keys final
	{
	public:
		std::vector<std::string> keys;
	};
}
