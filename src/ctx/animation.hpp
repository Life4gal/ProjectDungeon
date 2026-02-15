// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/animation.hpp>

#include <entt/fwd.hpp>

namespace pd::ctx
{
	class Animation final
	{
	public:
		// 添加新的动画集
		static auto add_set(entt::registry& registry, blueprint::AnimationSet&& sub_set) noexcept -> void;

		// 获取动画集(应该仅在systems::initialize::asset中使用)
		[[nodiscard]] static auto get_set(entt::registry& registry) noexcept -> const blueprint::AnimationSet&;

		[[nodiscard]] static auto get(entt::registry& registry, std::string_view name) noexcept -> blueprint::AnimationView;

		// 暂停指定实体的动画
		static auto pause(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 恢复指定实体的动画
		static auto unpause(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 使一个动画循环
		static auto looping(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 使一个动画不再循环(依然会播放到最后一帧,然后不再更新)
		static auto unlooping(entt::registry& registry, entt::entity entity) noexcept -> void;
	};
}
