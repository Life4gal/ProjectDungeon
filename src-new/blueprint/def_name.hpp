// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/def.hpp>

#include <prometheus/meta/enumeration.hpp>

namespace prometheus::meta::user_defined
{
	template<>
	struct enum_name_policy<pd::blueprint::Direction>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_name_policy<pd::blueprint::DirectionMask>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_name_policy<pd::blueprint::RenderLayer>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_name_policy<pd::blueprint::CollisionCategory>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_name_policy<pd::blueprint::CollisionMask>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};

	template<>
	struct enum_name_policy<pd::blueprint::EnemyType>
	{
		constexpr static auto value = EnumNamePolicy::WITH_SCOPED_NAME;
	};
}
