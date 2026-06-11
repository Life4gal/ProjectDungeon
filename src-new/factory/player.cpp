// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/player.hpp>

#include <component/player.hpp>

#include <assembly/character.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::factory
{
	using namespace component;

	auto Player::spawn(entt::registry& registry, const blueprint::Player& player) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// character
		assembly::Character::make(registry, entity, player.character);
		// speed
		registry.emplace<player::Speed>(entity, player.speed);
		// tags
		registry.emplace<tags::Player>(entity);

		// 新生成
		registry.emplace<state::EntityNew>(entity);

		return entity;
	}

	auto Player::destroy_all(entt::registry& registry) noexcept -> void
	{
		SPDLOG_INFO("正在销毁所有玩家...");

		const auto view = registry.view<tags::Player>();
		registry.destroy(view.begin(), view.end());

		SPDLOG_INFO("已销毁{}个玩家", view.size());
	}
}
