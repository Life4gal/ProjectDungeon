// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/player_status.hpp>

#include <manager/resource.hpp>

#include <component/name.hpp>
#include <component/property.hpp>

#include <accessor/player_controller.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	namespace
	{
		// HUD开始位置
		constexpr float HudBeginX = 10;
		constexpr float HudBeginY = 10;
		// HUD字体
		constexpr std::string_view HudFont = R"(C:\Windows\Fonts\msyh.ttc)";
		// HUD字体大小
		constexpr int HudFontSize = 30;
		// 生命值条&魔法值条位置
		constexpr float HudStatusBarBeginX = HudBeginX;
		constexpr float HudStatusBarBeginY = HudBeginY + 5;
		// 生命值条&魔法值条大小
		constexpr float HudStatusBarWidth = 120;
		constexpr float HudStatusBarHeight = 20;

		constexpr sf::Vector2f HudStatusBarPositionHealth{HudStatusBarBeginX, HudStatusBarBeginY + HudFontSize};
		constexpr sf::Vector2f HudStatusBarPositionMana{HudStatusBarBeginX, HudStatusBarPositionHealth.y + HudStatusBarHeight + 3};
		constexpr sf::Vector2f HudStatusBarSize{HudStatusBarWidth, HudStatusBarHeight};
	}

	auto player_status(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		if (not accessor::PlayerController::online(registry))
		{
			return;
		}

		const auto target = accessor::PlayerController::target(registry);
		if (not registry.valid(target))
		{
			return;
		}

		const auto* name = registry.try_get<name::Name>(target);
		if (name == nullptr)
		{
			return;
		}

		// 名字
		const auto font = manager::Font::load(HudFont);
		sf::Text text{*font, sf::String::fromUtf8(name->name.begin(), name->name.end()), HudFontSize};
		text.setFillColor(sf::Color::Red);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1);
		text.setPosition({HudBeginX, HudBeginY});
		window.draw(text);

		// 生命值条&魔法值条
		if (const auto [health, health_max] = registry.try_get<property::Health, property::HealthMax>(target);
			health != nullptr && health_max != nullptr)
		{
			const auto [mana, mana_max] = registry.try_get<property::Mana, property::ManaMax>(target);
			const auto has_mana = mana != nullptr and mana_max != nullptr;

			// 2个矩形(背景+前景) * 6个顶点(每个矩形2个三角形) = 12
			// 生命值条+魔法值条 = 2
			sf::VertexArray triangles{sf::PrimitiveType::Triangles, 12uz * (1 + (has_mana ? 1 : 0))};

			// 生命值条
			{
				constexpr auto health_bar_background_color = sf::Color::Red;

				const auto health_ratio = health->health / health_max->health;
				const auto health_color = health_ratio > 0.6f ? sf::Color::Green : sf::Color::Yellow;
				const auto health_size = sf::Vector2f{HudStatusBarWidth * health_ratio, HudStatusBarHeight};

				// background
				triangles.append({.position = HudStatusBarPositionHealth, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + sf::Vector2f{HudStatusBarWidth, 0}, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + HudStatusBarSize, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + HudStatusBarSize, .color = health_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + sf::Vector2f{0, HudStatusBarHeight}, .color = health_bar_background_color, .texCoords = {}});

				// health
				triangles.append({.position = HudStatusBarPositionHealth, .color = health_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + sf::Vector2f{health_size.x, 0}, .color = health_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + health_size, .color = health_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth, .color = health_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + health_size, .color = health_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionHealth + sf::Vector2f{0, health_size.y}, .color = health_color, .texCoords = {}});
			}

			// 魔法值条
			if (has_mana)
			{
				constexpr auto mana_bar_background_color = sf::Color::Black;

				const auto mana_ratio = mana->mana / mana_max->mana;
				constexpr auto mana_color = sf::Color::Blue;
				const auto mana_size = sf::Vector2f{HudStatusBarWidth * mana_ratio, HudStatusBarHeight};

				// background
				triangles.append({.position = HudStatusBarPositionMana, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + sf::Vector2f{HudStatusBarWidth, 0}, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + HudStatusBarSize, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + HudStatusBarSize, .color = mana_bar_background_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + sf::Vector2f{0, HudStatusBarHeight}, .color = mana_bar_background_color, .texCoords = {}});

				// mana
				triangles.append({.position = HudStatusBarPositionMana, .color = mana_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + sf::Vector2f{mana_size.x, 0}, .color = mana_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + mana_size, .color = mana_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana, .color = mana_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + mana_size, .color = mana_color, .texCoords = {}});
				triangles.append({.position = HudStatusBarPositionMana + sf::Vector2f{0, mana_size.y}, .color = mana_color, .texCoords = {}});
			}

			window.draw(triangles);
		}
	}
}
