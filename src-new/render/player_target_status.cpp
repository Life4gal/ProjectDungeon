// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/player_target_status.hpp>

#include <manager/resource.hpp>
#include <manager/clock.hpp>

#include <var/window.hpp>

#include <component/name.hpp>
#include <component/property.hpp>
#include <component/damage_statistics.hpp>

#include <accessor/player_controller.hpp>

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
		// constexpr float HudBeginX = 10;
		constexpr float HudBeginY = 10;
		// HUD字体
		constexpr std::string_view HudFont = R"(C:\Windows\Fonts\msyh.ttc)";
		// HUD字体大小
		constexpr int HudFontSize = 30;
		// 生命值条&魔法值条位置
		// X取决于状态条宽度和屏幕大小
		// constexpr float HudStatusBarBeginX = HudBeginX;
		// Y取决于第几个元素
		// constexpr float HudStatusBarBeginY = HudBeginY + 5;
		// 生命值条&魔法值条大小
		constexpr float HudStatusBarWidth = 120;
		constexpr float HudStatusBarHeight = 20;

		// X取决于状态条宽度和屏幕大小
		constexpr sf::Vector2f HudStatusBarSize{HudStatusBarWidth, HudStatusBarHeight};
	}

	auto player_target_status(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
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

		const auto* records = registry.try_get<const damage_statistics::AttackRecords>(target);
		if (records == nullptr or records->records.empty())
		{
			return;
		}

		const auto victims = [&] noexcept -> std::unordered_set<entt::entity>
		{
			// 仅考虑最近5秒的记录
			constexpr auto max_interval = sf::seconds(5);
			const auto now = manager::Clock::now();
			// 最多3个元素
			constexpr auto max_element = 3;
			// 去重
			std::unordered_set<entt::entity> result{};
			result.reserve(max_element);

			for (const auto& record: records->records | std::views::reverse)
			{
				if (now - record.time > max_interval)
				{
					// 直接结束
					break;
				}

				if (not registry.valid(record.victim))
				{
					// 无效的跳过
					continue;
				}

				if (const auto [it, inserted] = result.emplace(record.victim);
					inserted)
				{
					if (result.size() >= max_element)
					{
						// 数量达标直接结束
						break;
					}
				}
			}

			return result;
		}();

		if (victims.empty())
		{
			return;
		}

		// 2个矩形(背景+前景) * 6个顶点(每个矩形2个三角形) = 12
		// 生命值条+魔法值条 = 2
		sf::VertexArray triangles{sf::PrimitiveType::Triangles, 12uz * 2 * victims.size()};

		// 起始X/Y
		const auto hud_begin_x = static_cast<float>(var::window_width) - HudStatusBarWidth - 20.f;
		float current_y = HudBeginY;

		for (const auto victim: victims)
		{
			// 名字
			const auto* name = registry.try_get<name::Name>(victim);
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
			text.setPosition({hud_begin_x, current_y});
			window.draw(text);

			current_y += HudFontSize + 5;

			// 生命值条&魔法值条
			if (const auto [health, health_max] = registry.try_get<property::Health, property::HealthMax>(victim);
				health != nullptr and health_max != nullptr)
			{
				const sf::Vector2f hud_status_bar_position_health{hud_begin_x, current_y};
				const sf::Vector2f hud_status_bar_position_mana{hud_begin_x, hud_status_bar_position_health.y + HudStatusBarHeight + 3};

				// 有生命值条
				// 增加Y轴
				current_y += HudStatusBarHeight;

				// 生命值条
				{
					constexpr auto health_bar_background_color = sf::Color::Red;

					const auto health_ratio = health->health / health_max->health;
					const auto health_color = health_ratio > 0.6f ? sf::Color::Green : sf::Color::Yellow;
					const auto health_size = sf::Vector2f{HudStatusBarWidth * health_ratio, HudStatusBarHeight};

					// background
					triangles.append({.position = hud_status_bar_position_health, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + sf::Vector2f{HudStatusBarWidth, 0}, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + HudStatusBarSize, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + HudStatusBarSize, .color = health_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + sf::Vector2f{0, HudStatusBarHeight}, .color = health_bar_background_color, .texCoords = {}});

					// health
					triangles.append({.position = hud_status_bar_position_health, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + sf::Vector2f{health_size.x, 0}, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + health_size, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + health_size, .color = health_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_health + sf::Vector2f{0, health_size.y}, .color = health_color, .texCoords = {}});
				}

				// 魔法值条
				if (const auto [mana, mana_max] = registry.try_get<property::Mana, property::ManaMax>(victim);
					mana != nullptr and mana_max != nullptr and mana->mana > 0)
				{
					// 有魔法值条
					// 增加Y轴
					current_y += 3 + HudStatusBarHeight;

					constexpr auto mana_bar_background_color = sf::Color::Black;

					const auto mana_ratio = mana->mana / mana_max->mana;
					constexpr auto mana_color = sf::Color::Blue;
					const auto mana_size = sf::Vector2f{HudStatusBarWidth * mana_ratio, HudStatusBarHeight};

					// background
					triangles.append({.position = hud_status_bar_position_mana, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + sf::Vector2f{HudStatusBarWidth, 0}, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + HudStatusBarSize, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + HudStatusBarSize, .color = mana_bar_background_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + sf::Vector2f{0, HudStatusBarHeight}, .color = mana_bar_background_color, .texCoords = {}});

					// mana
					triangles.append({.position = hud_status_bar_position_mana, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + sf::Vector2f{mana_size.x, 0}, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + mana_size, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + mana_size, .color = mana_color, .texCoords = {}});
					triangles.append({.position = hud_status_bar_position_mana + sf::Vector2f{0, mana_size.y}, .color = mana_color, .texCoords = {}});
				}
			}

			// 增加间隔
			current_y += 5;
		}

		window.draw(triangles);
	}
}
