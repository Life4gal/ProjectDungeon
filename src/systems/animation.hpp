// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <systems/system.hpp>

#include <blueprint/animation.hpp>

namespace pd::systems
{
	class Animation final : public System<Animation>
	{
		friend System;

		static auto do_loaded(entt::registry& registry) noexcept -> void;

		static auto do_initialized(entt::registry& registry) noexcept -> void;

		static auto do_unloaded(entt::registry& registry) noexcept -> void;

		static auto do_update(entt::registry& registry, sf::Time delta) noexcept -> void;

		static auto do_render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;

	public:
		// ========================================
		// 定义一套static接口,便于直接读取registry.ctx
		// ========================================

		// 获取指定动画
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
