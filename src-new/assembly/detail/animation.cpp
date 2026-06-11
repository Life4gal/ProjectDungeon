// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/detail/animation.hpp>

#include <ranges>

#include <component/animation.hpp>

#include <assembly/render.hpp>

#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	namespace
	{
		[[nodiscard]] constexpr auto group_of(const blueprint::Animation::groups_type& groups) noexcept -> animation::Groups::groups_type
		{
			// return
			// 		groups |
			// 		std::views::transform(
			// 			[](const blueprint::Animation::Group& group) noexcept-> animation::Groups::Group
			// 			{
			// 				return {.begin_frame = group.begin_frame, .end_frame = group.end_frame};
			// 			}
			// 		) |
			// 		std::ranges::to<animation::Groups::groups_type>();

			animation::Groups::groups_type a_groups{};

			for (auto [group, a_group]: std::views::zip(groups, a_groups))
			{
				a_group = {.begin_frame = group.begin_frame, .end_frame = group.end_frame};
			}

			return a_groups;
		}
	}

	auto Animation::make(entt::registry& registry, const entt::entity entity, const blueprint::Animation& animation) noexcept -> void
	{
		// ================
		// 渲染
		// ================

		Render::make(registry, entity, animation.sprite_sheet);

		// ================
		// 动画
		// ================

		// 朝向
		// 默认向下
		registry.emplace<animation::Direction>(entity, animation::Direction::SOUTH);
		// 索引
		registry.emplace<animation::Groups>(entity, group_of(animation.groups));
	}
}
