// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/def.hpp>

#include <prometheus/meta/enumeration.hpp>

namespace prometheus::meta::user_defined
{
	// ==============
	// DIRECTION
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::Direction>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	// ==============
	// DIRECTION MASK
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::DirectionMask>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_is_flag<pd::blueprint::DirectionMask> : std::true_type {};

	// ==============
	// ANIMATION
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::AnimationMode>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_name_policy<pd::blueprint::AnimationDirection>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	// ==============
	// RENDER LAYER
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::RenderLayer>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_is_flag<pd::blueprint::RenderLayer> : std::true_type {};

	// ==============
	// COLLISION CATEGORY
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::CollisionCategory>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_is_flag<pd::blueprint::CollisionCategory> : std::true_type {};

	// ==============
	// COLLISION MASK
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::CollisionMask>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_is_flag<pd::blueprint::CollisionMask> : std::true_type {};

	// ==============
	// ENEMY TYPE
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::EnemyType>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	// ==============
	// NPC TYPE
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::NpcType>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	// ==============
	// NAME TYPE
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::NameType>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	// ==============
	// ROOM TYPE
	// ==============

	template<>
	struct enum_name_policy<pd::blueprint::RoomType>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	// ==============
	// 
	// ==============
}
