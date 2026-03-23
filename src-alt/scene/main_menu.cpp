// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main_menu.hpp>

#include <algorithm>
#include <ranges>

#include <components/game_config.hpp>

#include <game/asset.hpp>

#include <systems/game_config.hpp>
#include <systems/asset_manager.hpp>

#include <prometheus/platform/os.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <components/asset_manager.hpp>

namespace pd::scene
{
	auto MainMenu::handle_event_main(const sf::Event& event) noexcept -> void
	{
		auto& registry = scene_registry_;

		const auto* kp = event.getIf<sf::Event::KeyPressed>();

		if (kp == nullptr)
		{
			return;
		}

		const auto action = [&] noexcept -> MenuAction
		{
			using namespace sf::Keyboard;

			if (kp->code == Key::Up)
			{
				return MenuAction::UP;
			}

			if (kp->code == Key::Down)
			{
				return MenuAction::DOWN;
			}

			if (kp->code == Key::Enter)
			{
				return MenuAction::CONFIRM;
			}

			if (kp->code == Key::Escape)
			{
				return MenuAction::CANCEL;
			}

			return MenuAction::NONE;
		}();

		if (action == MenuAction::NONE)
		{
			return;
		}

		if (action == MenuAction::UP or action == MenuAction::DOWN)
		{
			systems::AssetManager::play_sound(registry, sound_id_switch_option_);

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
			const auto selected_option = static_cast<MenuOption>(selected_option_value_);
			if (selected_option == MenuOption::CONTINUE)
			{
				//
			}
			else if (selected_option == MenuOption::START)
			{
				// todo: 如何切换场景?
				// 发送切换场景事件?
			}
			else if (selected_option == MenuOption::OPTIONS)
			{
				//
			}
			else if (selected_option == MenuOption::QUIT)
			{
				// todo: 如何退出游戏?
				// 发送退出游戏事件?
			}
		}
		else if (action == MenuAction::CANCEL)
		{
			// todo: 如何退出游戏?
			// 发送退出游戏事件?
		}
		else
		{
			PROMETHEUS_PLATFORM_UNREACHABLE();
		}
	}

	auto MainMenu::render_main(sf::RenderWindow& window) noexcept -> void
	{
		auto& registry = scene_registry_;

		const auto& font = systems::AssetManager::get_font(registry, font_id_);

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

	MainMenu::MainMenu(const std::reference_wrapper<entt::registry> global_registry) noexcept
		: Scene{global_registry},
		  font_id_{manager::invalid_asset_id},
		  sound_id_switch_option_{manager::invalid_asset_id},
		  music_id_{manager::invalid_asset_id},
		  selected_option_value_{std::to_underlying(MenuOption::START)} {}

	auto MainMenu::on_loaded() noexcept -> void
	{
		auto& registry = scene_registry_;

		// 直接复制global_registry的GameConfig
		{
			PROMETHEUS_PLATFORM_ASSUME(global_registry_.get().ctx().contains<components::GameConfig>());
			registry.ctx().emplace<components::GameConfig>(global_registry_.get().ctx().get<const components::GameConfig>());
		}

		registry.ctx().emplace<components::FontManager>();
		registry.ctx().emplace<components::TextureManager>();
		registry.ctx().emplace<components::SoundManager>();
		registry.ctx().emplace<components::MusicManager>();

		// SoundChannels
		registry.ctx().emplace<components::SoundChannels>();
		// MusicChannel
		registry.ctx().emplace<components::MusicChannel>(nullptr);

		font_id_ = systems::AssetManager::load_font(registry, game::fonts[std::to_underlying(game::Fonts::MAIN_MENU)]);
		sound_id_switch_option_ = systems::AssetManager::load_sound(registry, game::sounds[std::to_underlying(game::Sounds::MAIN_MENU_SWITCH_OPTION)]);
		music_id_ = systems::AssetManager::load_music(registry, game::musics[std::to_underlying(game::Musics::MAIN_MENU)]);
		player_name_label_ = std::string{"Player Name: "}.append(systems::GameConfig::get_player_name(registry));
		{
			// 如果存在存档则为continue,否则为start
			selected_option_value_ = std::to_underlying(MenuOption::START);
		}
	}

	auto MainMenu::on_initialized() noexcept -> void
	{
		auto& registry = scene_registry_;

		systems::AssetManager::play_music(registry, music_id_);
	}

	auto MainMenu::on_unloaded() noexcept -> void
	{
		auto& registry = scene_registry_;

		// 将GameConfig复制回global_registry
		{
			PROMETHEUS_PLATFORM_ASSUME(global_registry_.get().ctx().contains<components::GameConfig>());
			PROMETHEUS_PLATFORM_ASSUME(scene_registry_.ctx().contains<components::GameConfig>());

			global_registry_.get().ctx().insert_or_assign(registry.ctx().get<const components::GameConfig>());
		}

		// registry销毁时会自动销毁上面创建的资源
		std::ignore = registry;
	}

	auto MainMenu::handle_event(const sf::Event& event) noexcept -> void
	{
		handle_event_main(event);
	}

	auto MainMenu::update(const sf::Time delta) noexcept -> void
	{
		//
	}

	auto MainMenu::render(sf::RenderWindow& window) noexcept -> void
	{
		render_main(window);
	}
}
