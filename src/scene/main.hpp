// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <scene/scene.hpp>

#include <systems/world.hpp>
#include <systems/blueprint.hpp>
#include <systems/asset.hpp>
#include <systems/flip_animation.hpp>

#include <systems/debug.hpp>

namespace pd::scene
{
	class Main final : public Scene
	{
	public:
		//

	private:
		// 这些系统基本都是(全都是?)纯静态,不过保留它们的实例也无所谓 :)
		systems::World world_system_;
		systems::Blueprint blueprint_system_;
		systems::Asset asset_system_;
		systems::FlipAnimation flip_animation_system_;

		systems::Debug debug_system_;

		// 开始一场游戏
		// 在一个游戏场景中可以开始多次游戏(例如死亡后重开)
		// 此时不需要切换场景,只需要打扫战场,重新生成对应实体即可
		auto start_game() noexcept -> void;

	public:
		explicit Main(std::reference_wrapper<entt::registry> global_registry) noexcept;

		auto on_loaded() noexcept -> void override;

		auto on_initialized() noexcept -> void override;

		auto on_unloaded() noexcept -> void override;

		auto handle_event(const sf::Event& event) noexcept -> void override;

		auto update(sf::Time delta) noexcept -> void override;

		auto render(sf::RenderWindow& window) noexcept -> void override;
	};
}
