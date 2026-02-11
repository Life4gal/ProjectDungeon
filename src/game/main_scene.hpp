// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <game/scene.hpp>

#include <box2d/id.h>

namespace pd
{
	class MainScene final : public Scene
	{
	public:
		//

	private:
		// 物理世界
		b2WorldId physics_world_id_;

		// 创建物理世界
		auto create_physics_world() noexcept -> void;

	public:
		explicit MainScene(std::reference_wrapper<entt::registry> global_registry) noexcept;

		MainScene(const MainScene&) noexcept = delete;
		auto operator=(const MainScene&) noexcept -> MainScene& = delete;

		MainScene(MainScene&&) noexcept = default;
		auto operator=(MainScene&&) noexcept -> MainScene& = default;

		~MainScene() noexcept override;

		auto handle_event(const sf::Event& event) noexcept -> void override;

		auto update(sf::Time delta) noexcept -> void override;

		auto render(sf::RenderWindow& window) noexcept -> void override;
	};
}
