// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::helper
{
	class Property final
	{
	public:
		// =====================================
		// TODO: 下面这几个接口放在这里显然不合适,应该放到哪里呢?

		// 对指定实体造成伤害,如果目标实体没有Actor组件则什么也不做
		//
		// Event:
		//  event::actor::Damaged
		static auto hurt(entt::registry& registry, entt::entity victim, entt::entity attacker, float damage) noexcept -> void;

		// 杀死指定实体,如果目标实体没有Actor组件则什么也不做
		//
		// Event:
		//  event::actor::Slayed
		static auto kill(entt::registry& registry, entt::entity victim, entt::entity attacker) noexcept -> void;
	};
}
