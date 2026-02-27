// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::systems::update
{
	// 检测当前房间敌人是否已清空
	// 如果清空则解锁所有不需要钥匙的门
	// todo: 有钥匙的如何处理?
	auto update_room(entt::registry& registry) noexcept -> void;
}
