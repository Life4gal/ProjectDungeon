// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/entity.hpp>

namespace pd
{
	[[nodiscard]] constexpr auto entity_to_user_data(const entt::entity entity) noexcept -> void*
	{
		return std::bit_cast<void*>(static_cast<std::uintptr_t>(entt::to_integral(entity)));
	}

	[[nodiscard]] constexpr auto user_data_to_entity(const void* user_data) noexcept -> entt::entity
	{
		return entt::entity{static_cast<std::underlying_type_t<entt::entity>>(std::bit_cast<std::uintptr_t>(user_data))};
	}
}
