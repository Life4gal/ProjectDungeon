// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <undertaker/collision.hpp>

#include <component/collision.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::undertaker
{
	using namespace component;

	namespace
	{
		auto on_destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
		{
			const auto [body_id] = registry.get<const collision::BodyId>(entity);
			b2DestroyBody(body_id);
		}
	}

	auto Collision::watch(entt::registry& registry) noexcept -> void
	{
		registry.on_destroy<collision::BodyId>().connect<&on_destroy>();
	}
}
