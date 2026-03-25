// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/entity.hpp>

namespace pd::utility
{
	enum class AssetId : entt::id_type {};

	constexpr auto invalid_asset_id = AssetId{entt::null};
}
