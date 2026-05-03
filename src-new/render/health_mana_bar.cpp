// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/health_mana_bar.hpp>

#include <component/actor.hpp>
#include <component/state.hpp>
#include <component/transform.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	auto health_mana_bar(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		const auto view = registry
				.view<
					state::InCameraArea,
					actor::Health,
					actor::HealthMax,
					actor::Mana,
					actor::ManaMax,
					actor::HealthBarSize,
					actor::HealthBarOffset,
					actor::ManaBarSize,
					actor::ManaBarOffset,
					transform::Position
				>(entt::exclude<actor::Dead>);
		const auto count = view.size_hint();

		// 2个矩形(背景+前景) * 6个顶点(每个矩形2个三角形) = 12
		// 生命值条+魔法值条 = 2
		sf::VertexArray triangles{sf::PrimitiveType::Triangles, count * 12 * 2};

		for (const auto [entity, health, health_max, mana, mana_max, health_bar_size, health_bar_offset, mana_bar_size, mana_bar_offset, position]: view.each())
		{
			// 生命值条
			{
				constexpr auto health_bar_background_color = sf::Color::Red;
				const auto health_bar_position = position.position + health_bar_offset.offset;

				const auto health_ratio = health.health / health_max.health;
				const auto health_color = health_ratio > 0.6f ? sf::Color::Green : sf::Color::Yellow;
				const auto health_size = sf::Vector2f{health_bar_size.size.x * health_ratio, health_bar_size.size.y};

				// background
				triangles.append({.position = health_bar_position, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + sf::Vector2f{health_bar_size.size.x, 0}, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + health_bar_size.size, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = health_bar_position, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + health_bar_size.size, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + sf::Vector2f{0, health_bar_size.size.y}, .color = health_bar_background_color, .texCoords = {}});

				// health
				triangles.append({.position = health_bar_position, .color = health_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + sf::Vector2f{health_size.x, 0}, .color = health_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + health_size, .color = health_color, .texCoords = {}});
				triangles.append({.position = health_bar_position, .color = health_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + health_size, .color = health_color, .texCoords = {}});
				triangles.append({.position = health_bar_position + sf::Vector2f{0, health_size.y}, .color = health_color, .texCoords = {}});
			}

			// 魔法值条
			// 当且仅当有魔法值时才绘制
			// FIXME: 此时需不需要减少triangles的顶点数量?
			if (mana_max.mana > 0)
			{
				constexpr auto mana_bar_background_color = sf::Color::Black;
				const auto mana_bar_position = position.position + mana_bar_offset.offset;

				const auto mana_ratio = mana.mana / mana_max.mana;
				constexpr auto mana_color = sf::Color::Blue;
				const auto mana_size = sf::Vector2f{mana_bar_size.size.x * mana_ratio, mana_bar_size.size.y};

				// background
				triangles.append({.position = mana_bar_position, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + sf::Vector2f{mana_bar_size.size.x, 0}, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + mana_bar_size.size, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + mana_bar_size.size, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + sf::Vector2f{0, mana_bar_size.size.y}, .color = mana_bar_background_color, .texCoords = {}});

				// mana
				triangles.append({.position = mana_bar_position, .color = mana_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + sf::Vector2f{mana_size.x, 0}, .color = mana_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + mana_size, .color = mana_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position, .color = mana_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + mana_size, .color = mana_color, .texCoords = {}});
				triangles.append({.position = mana_bar_position + sf::Vector2f{0, mana_size.y}, .color = mana_color, .texCoords = {}});
			}
		}

		window.draw(triangles);
	}
}
