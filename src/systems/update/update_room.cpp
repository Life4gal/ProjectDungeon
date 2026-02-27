// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/update_room.hpp>

#include <systems/helper/level.hpp>

namespace pd::systems::update
{
	auto update_room(entt::registry& registry) noexcept -> void
	{
		using namespace systems;

		helper::Level::update_room(registry);
	}
} // namespace pd::systems::update
