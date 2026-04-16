// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/room.hpp>

#include <manager/asset.hpp>

#include <components/tags.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>

#include <components/room.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace pd::render
{
	namespace
	{
		using namespace components;

		auto render_tile(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			//
		}

		auto render_door(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			//
		}

		auto render_item(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			//
		}

		auto render_chest(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			//
		}

		auto render_blood_stain(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			//
		}

		auto render_corpse(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			//
		}

		auto render_destroyable(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			//
		}
	}

	auto room(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		// 先渲染瓦片(地板/墙壁)
		render_tile(registry, window);

		// 再渲染血迹
		render_blood_stain(registry, window);

		// 再渲染门
		render_door(registry, window);

		// 再渲染尸体
		render_corpse(registry, window);

		// 再渲染可破坏物
		render_destroyable(registry, window);

		// 再渲染宝箱
		render_chest(registry, window);

		// 最后渲染物品
		render_item(registry, window);
	}
}
