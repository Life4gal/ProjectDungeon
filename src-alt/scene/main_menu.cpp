// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main_menu.hpp>

#include <ranges>

#include <game/menu_action.hpp>
#include <game/asset.hpp>

#include <components/asset_manager.hpp>

#include <systems/game_config.hpp>
#include <systems/dispatcher.hpp>

#include <systems/asset_manager.hpp>

#include <prometheus/platform/os.hpp>

#include <SFML/Graphics.hpp>

namespace pd::scene
{
	auto MainMenu::handle_event_main(const sf::Event& event) noexcept -> void
	{
		using namespace game;
		using namespace systems;

		const auto* kp = event.getIf<sf::Event::KeyPressed>();

		if (kp == nullptr)
		{
			return;
		}

		const auto action = MenuActionMap::get(kp->code);

		if (
			action != MenuAction::UP and
			action != MenuAction::DOWN and
			action != MenuAction::CONFIRM and
			action != MenuAction::CANCEL
		)
		{
			return;
		}

		if (action == MenuAction::UP or action == MenuAction::DOWN)
		{
			AssetManager::play_sound(registry_, sound_id_switch_option_);

			if (action == MenuAction::UP)
			{
				if (selected_option_value_ == 0)
				{
					selected_option_value_ = std::to_underlying(MenuOption::COUNT) - 1;
				}
				else
				{
					selected_option_value_ -= 1;
				}
			}
			else
			{
				if (selected_option_value_ == std::to_underlying(MenuOption::COUNT) - 1)
				{
					selected_option_value_ = 0;
				}
				else
				{
					selected_option_value_ += 1;
				}
			}
		}
		else if (action == MenuAction::CONFIRM)
		{
			if (const auto selected_option = static_cast<MenuOption>(selected_option_value_);
				selected_option == MenuOption::CONTINUE)
			{
				// todo
				Dispatcher::scene_change(registry_, Scenes::GAME);
			}
			else if (selected_option == MenuOption::START)
			{
				Dispatcher::scene_change(registry_, Scenes::GAME);
			}
			else if (selected_option == MenuOption::OPTIONS)
			{
				// todo
			}
			else if (selected_option == MenuOption::QUIT)
			{
				Dispatcher::scene_change(registry_, Scenes::QUIT);
			}
		}
		else if (action == MenuAction::CANCEL)
		{
			Dispatcher::scene_change(registry_, Scenes::QUIT);
		}
		else
		{
			PROMETHEUS_PLATFORM_UNREACHABLE();
		}
	}

	auto MainMenu::render_main(sf::RenderWindow& window) noexcept -> void
	{
		using namespace systems;

		const auto& font = AssetManager::get_font(registry_, font_id_);

		constexpr float menu_x = 290.f;
		constexpr float menu_y_start = 300.f;
		constexpr float menu_y_step = 36.f;
		constexpr auto menu_color_normal = sf::Color{120, 120, 120, 255};
		constexpr auto menu_color_selected = sf::Color{255, 255, 255, 255};

		const std::string menu_option_labels[]
		{
				player_name_label_,
				"Continue",
				"Star New Game",
				"Options",
				"Quit Game",
		};

		for (const auto [index, label]: menu_option_labels | std::views::enumerate)
		{
			const auto color = std::cmp_equal(index, selected_option_value_) ? menu_color_selected : menu_color_normal;

			sf::Text text{*font, label, 24};
			text.setFillColor(color);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(1);
			text.setPosition({menu_x, menu_y_start + static_cast<float>(index) * menu_y_step});

			window.draw(text);
		}
	}

	MainMenu::MainMenu(Game& game) noexcept
		: Scene{game},
		  font_id_{manager::invalid_asset_id},
		  sound_id_switch_option_{manager::invalid_asset_id},
		  music_id_{manager::invalid_asset_id},
		  selected_option_value_{std::to_underlying(MenuOption::START)} {}

	auto MainMenu::on_loaded() noexcept -> void
	{
		using namespace game;
		using namespace systems;

		// AssetManager
		{
			registry_.ctx().emplace<components::FontManager>();
			registry_.ctx().emplace<components::TextureManager>();
			registry_.ctx().emplace<components::SoundManager>();
			registry_.ctx().emplace<components::MusicManager>();

			// SoundChannels
			registry_.ctx().emplace<components::SoundChannels>();
			// MusicChannel
			registry_.ctx().emplace<components::MusicChannel>(nullptr);
		}

		font_id_ = AssetManager::load_font(registry_, Font::get(Fonts::MAIN_MENU));
		sound_id_switch_option_ = AssetManager::load_sound(registry_, Sound::get(Sounds::MENU_SWITCH_OPTION));
		music_id_ = AssetManager::load_music(registry_, Music::get(Musics::MAIN_MENU));
		player_name_label_ = std::string{"Player Name: "}.append(GameConfig::get_player_name(registry_));
		{
			// 如果存在存档则为continue,否则为start
			selected_option_value_ = std::to_underlying(MenuOption::START);
		}
	}

	auto MainMenu::on_initialized() noexcept -> void
	{
		using namespace systems;

		AssetManager::play_music(registry_, music_id_);
	}

	auto MainMenu::on_unloaded() noexcept -> void
	{
		using namespace systems;

		AssetManager::stop_music(registry_, music_id_);
	}

	auto MainMenu::handle_event(const sf::Event& event) noexcept -> void
	{
		using namespace systems;

		handle_event_main(event);
	}

	auto MainMenu::update(const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		//
	}

	auto MainMenu::render(sf::RenderWindow& window) noexcept -> void
	{
		using namespace systems;

		render_main(window);
	}
}
