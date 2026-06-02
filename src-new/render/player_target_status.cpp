// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/player_target_status.hpp>

#include <ranges>

#include <manager/resource.hpp>
#include <manager/clock.hpp>

#include <var/window.hpp>

#include <component/name.hpp>
#include <component/property.hpp>
#include <component/damage_statistics.hpp>

#include <helper/player_controller.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	namespace
	{
		// 默认名称
		constexpr std::string_view HudDefaultName = "???";
		// HUD开始位置
		// X取决于状态条宽度和屏幕大小
		// constexpr float HudBeginX = 0
		constexpr float HudBeginY = 20.f;
		// HUD字体
		constexpr std::string_view HudFont = R"(C:\Windows\Fonts\msyh.ttc)";
		// HUD字体大小
		constexpr int HudFontSize = 30;
		// 生命值条&魔法值条位置
		// X取决于状态条宽度和屏幕大小
		// constexpr float HudStatusBarX = HudBeginX;
		// Y取决于第几个元素
		// constexpr float HudStatusBarY = HudBeginY + HudFontSize + 5;
		// 生命值条&魔法值条大小
		constexpr float HudStatusBarWidth = 80;
		constexpr float HudStatusBarHeight = 20;

		// X取决于状态条宽度和屏幕大小
		// constexpr sf::Vector2f HudStatusBarPosition1{0, HudStatusBarY};
		// constexpr sf::Vector2f HudStatusBarPosition2{0, HudStatusBarY + HudStatusBarHeight + 3};
		constexpr sf::Vector2f HudStatusBarSize{HudStatusBarWidth, HudStatusBarHeight};
	}

	auto player_target_status(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		if (not helper::PlayerController::online(registry))
		{
			return;
		}

		const auto target = helper::PlayerController::target(registry);
		if (not registry.valid(target))
		{
			return;
		}

		const auto* records = registry.try_get<const damage_statistics::AttackRecords>(target);
		if (records == nullptr or records->records.empty())
		{
			return;
		}

		// 仅考虑最近5秒的记录
		const auto now = manager::Clock::now();
		auto less_than_5_seconds =
				records->records | //
				std::views::reverse | //
				std::views::take_while(
					[now](const damage_statistics::AttackRecords::Record& record) noexcept -> bool
					{
						return now - record.time <= sf::seconds(5);
					}
				);
		// 低于五秒
		const auto less_than_5_seconds_count = std::ranges::distance(less_than_5_seconds);
		if (less_than_5_seconds_count == 0)
		{
			return;
		}

		// 去除重复
		std::unordered_set<entt::entity> used_victims{};
		auto unique =
				less_than_5_seconds | //
				std::views::filter(
					[&used_victims](const damage_statistics::AttackRecords::Record& record) noexcept -> bool
					{
						return used_victims.insert(record.victim).second;
					}
				);

		// 最多3个元素
		auto last_3_elements =
				unique | //
				std::views::take(3);
		const auto last_3_elements_count = std::ranges::distance(last_3_elements);

		// 2个矩形(背景+前景) * 6个顶点(每个矩形2个三角形) = 12
		// 生命值条+魔法值条 = 2
		sf::VertexArray triangles{sf::PrimitiveType::Triangles, 12uz * 2 * last_3_elements_count};

		// 起始X/Y
		const auto hud_begin_x = static_cast<float>(var::window_width) - HudStatusBarWidth - 20.f;
		float current_y = HudBeginY;

		for (const auto& record: last_3_elements)
		{
			if (not registry.valid(record.victim))
			{
				continue;
			}

			const sf::Vector2f hud_status_bar_position1{hud_begin_x, current_y};
			const sf::Vector2f hud_status_bar_position2{hud_begin_x, current_y + HudStatusBarHeight + 3};

			// 名字
			const auto* name = registry.try_get<name::Name>(record.victim);
			const auto font = manager::Font::load(HudFont);
			sf::Text text
			{
					*font,
					name ? sf::String::fromUtf8(name->name.begin(), name->name.end()) : sf::String::fromUtf8(HudDefaultName.begin(), HudDefaultName.end()),
					HudFontSize
			};
			text.setFillColor(sf::Color::Red);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(1);
			text.setPosition({hud_begin_x, current_y + HudFontSize});
			window.draw(text);

			// 生命值条&魔法值条
			if (const auto [health, health_max] = registry.try_get<property::Health, property::HealthMax>(record.victim);
				health != nullptr and health_max != nullptr)
			{
				// 生命值条
				{
					constexpr auto health_bar_background_color = sf::Color::Red;

					const auto health_ratio = health->health / health_max->health;
					const auto health_color = health_ratio > 0.6f ? sf::Color::Green : sf::Color::Yellow;
					const auto health_size = sf::Vector2f{HudStatusBarWidth * health_ratio, HudStatusBarHeight};

					// background
					triangles.append({.position = hud_status_bar_position1, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + sf::Vector2f{HudStatusBarWidth, 0}, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + HudStatusBarSize, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + HudStatusBarSize, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + sf::Vector2f{0, HudStatusBarHeight}, .color = health_bar_background_color, .texCoords = {}});

					// health
					triangles.append({.position = hud_status_bar_position1, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + sf::Vector2f{health_size.x, 0}, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + health_size, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + health_size, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position1 + sf::Vector2f{0, health_size.y}, .color = health_color, .texCoords = {}});
				}

				current_y += HudStatusBarHeight;

				// 魔法值条
				if (const auto [mana, mana_max] = registry.try_get<property::Mana, property::ManaMax>(record.victim);
					mana != nullptr and mana_max != nullptr)
				{
					current_y += 3;

					constexpr auto mana_bar_background_color = sf::Color::Black;

					const auto mana_ratio = mana->mana / mana_max->mana;
					constexpr auto mana_color = sf::Color::Blue;
					const auto mana_size = sf::Vector2f{HudStatusBarWidth * mana_ratio, HudStatusBarHeight};

					// background
					triangles.append({.position = hud_status_bar_position2, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + sf::Vector2f{HudStatusBarWidth, 0}, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + HudStatusBarSize, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + HudStatusBarSize, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + sf::Vector2f{0, HudStatusBarHeight}, .color = mana_bar_background_color, .texCoords = {}});

					// mana
					triangles.append({.position = hud_status_bar_position2, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + sf::Vector2f{mana_size.x, 0}, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + mana_size, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + mana_size, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position2 + sf::Vector2f{0, mana_size.y}, .color = mana_color, .texCoords = {}});

					current_y += HudStatusBarHeight;
				}
			}

			current_y += 5;
		}

		window.draw(triangles);
	}
}
