// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <game/scene.hpp>

namespace pd
{
	Scene::Scene(const std::reference_wrapper<entt::registry> global_registry) noexcept
		: global_registry_{global_registry} {}

	Scene::~Scene() noexcept = default;
}
