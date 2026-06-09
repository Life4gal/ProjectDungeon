// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/render.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::assembly
{
	class Render final
	{
	public:
		static auto make(entt::registry& registry, entt::entity entity, const blueprint::Sprite::Static& static_sprite) noexcept -> void;

		static auto make(entt::registry& registry, entt::entity entity, const blueprint::Sprite::Dynamic& dynamic_sprite) noexcept -> void;

		static auto make(entt::registry& registry, entt::entity entity, const blueprint::Sprite& sprite) noexcept -> void;

		// ========================================================================

		// 其他渲染方式?
	};
}
