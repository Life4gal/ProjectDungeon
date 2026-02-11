// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/registry.hpp>

#include <SFML/System/Time.hpp>

namespace sf
{
	class Event;
	class RenderWindow;
}

namespace pd
{
	class Scene
	{
	protected:
		// 全局实体世界
		std::reference_wrapper<entt::registry> global_registry_;
		// 此场景实体世界
		entt::registry scene_registry_;

	public:
		explicit Scene(std::reference_wrapper<entt::registry> global_registry) noexcept;

		Scene(const Scene&) noexcept = delete;
		auto operator=(const Scene&) noexcept -> Scene& = delete;

		Scene(Scene&&) noexcept = default;
		auto operator=(Scene&&) noexcept -> Scene& = default;

		virtual ~Scene() noexcept;

		virtual auto handle_event(const sf::Event& event) noexcept -> void = 0;

		virtual auto update(sf::Time delta) noexcept -> void = 0;

		virtual auto render(sf::RenderWindow& window) noexcept -> void = 0;
	};
}
