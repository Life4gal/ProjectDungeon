// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/core/type_traits.hpp>
#include <entt/core/hashed_string.hpp>

namespace pd::components::tags
{
	using entt::literals::operator ""_hs;

	// =================================================
	// 身份标识
	// =================================================

	// =================
	// 房间

	// 瓦片
	using tile = entt::tag<"Tile"_hs>;
	// 门
	using door = entt::tag<"Door"_hs>;
	// 宝箱
	using chest = entt::tag<"Chest"_hs>;
	// 可破坏物
	using destroyable_object = entt::tag<"DestroyableObject"_hs>;
	// 物品
	using item = entt::tag<"Item"_hs>;
	// 尸体
	using corpse = entt::tag<"Corpse"_hs>;
	// 血迹
	using blood_stain = entt::tag<"BloodStain"_hs>;

	// =================================================
	// 状态标识
	// =================================================

	// 禁用
	// FIXME(OPT): 这个标签目前威力太大了,是否需要根据具体功能拆分?
	using disabled = entt::tag<"Disabled"_hs>;

	// 濒死
	using dying = entt::tag<"Dying"_hs>;
	// 死亡
	using dead = entt::tag<"Dead"_hs>;
}
