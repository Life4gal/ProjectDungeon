// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/blueprint.hpp>

#include <algorithm>
#include <ranges>

// =====================
// CONFIG
// =====================

#include <config/animation.hpp>

// =====================
// BLUEPRINT
// =====================

#include <blueprint/animation.hpp>

// =====================
// ASSET
// =====================

#include <asset/manager.hpp>

// =====================
// DEPENDENCIES
// =====================

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Blueprint::do_loaded(entt::registry& registry) noexcept -> void
	{
		// 创建map
		// 这些实体由systems::Asset使用
		auto& font_map = registry.ctx().emplace<asset::FontMap>();
		auto& texture_map = registry.ctx().emplace<asset::TextureMap>();
		auto& sound_map = registry.ctx().emplace<asset::SoundMap>();
		auto& music_map = registry.ctx().emplace<asset::MusicMap>();

		// 读取数据
		const auto animation_config = config::load_animation();

		// 载入蓝图
		auto animation_blueprint = blueprint::load_animation(animation_config, texture_map);

		// 创建实体
		// 该实体由systems::Animation使用
		registry.ctx().emplace<blueprint::AnimationSet>(std::move(animation_blueprint));
	}

	auto Blueprint::do_initialized([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Blueprint::do_unloaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Blueprint::do_update([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		// 什么也不做
	}

	auto Blueprint::do_render([[maybe_unused]] entt::registry& registry, [[maybe_unused]] sf::RenderWindow& window) noexcept -> void
	{
		// 什么也不做
	}
}
