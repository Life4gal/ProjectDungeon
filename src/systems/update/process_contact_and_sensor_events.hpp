// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Time.hpp>

namespace pd::systems::update
{
	auto process_contact_and_sensor_events(entt::registry& registry, sf::Time delta) noexcept -> void;
}
