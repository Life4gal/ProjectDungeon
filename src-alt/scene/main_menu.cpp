// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main_menu.hpp>

#include <ranges>

#include <game/constants_map.hpp>

#include <events/scene.hpp>

#include <manager/event.hpp>
#include <manager/asset.hpp>

#include <prometheus/platform/os.hpp>

#include <SFML/Graphics.hpp>
#include <imgui.h>

namespace pd::scene
{
	auto MainMenu::handle_event_main(const sf::Event& event) noexcept -> void
	{
		using namespace game;
		using namespace manager;

		const auto* kp = event.getIf<sf::Event::KeyPressed>();

		if (kp == nullptr)
		{
			return;
		}

		const auto action = map(kp->code);

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
			manager::Sound::play(sound_id_switch_option_);

			if (action == MenuAction::UP)
			{
				if (selected_option_value_ == 0)
				{
					selected_option_value_ = std::to_underlying(option_type::COUNT) - 1;
				}
				else
				{
					selected_option_value_ -= 1;
				}
			}
			else
			{
				if (selected_option_value_ == std::to_underlying(option_type::COUNT) - 1)
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
			if (const auto selected_option = static_cast<option_type>(selected_option_value_);
				selected_option == option_type::CONTINUE)
			{
				// todo
				Event::enqueue(events::SceneChanged{.to = Type::GAME});
			}
			else if (selected_option == option_type::START)
			{
				Event::enqueue(events::SceneChanged{.to = Type::GAME});
			}
			else if (selected_option == option_type::OPTIONS)
			{
				// todo
			}
			else if (selected_option == option_type::QUIT)
			{
				Event::enqueue(events::SceneChanged{.to = Type::QUIT});
			}
		}
		else if (action == MenuAction::CANCEL)
		{
			Event::enqueue(events::SceneChanged{.to = Type::QUIT});
		}
		else
		{
			PROMETHEUS_PLATFORM_UNREACHABLE();
		}
	}

	auto MainMenu::render_main(sf::RenderWindow& window) const noexcept -> void
	{
		using namespace game;
		using namespace manager;

		const auto& font = manager::Font::get(font_id_);

		for (const auto option_value: std::views::iota(static_cast<std::underlying_type_t<option_type>>(0), std::to_underlying(option_type::COUNT)))
		{
			const auto option = static_cast<option_type>(option_value);
			const auto name = map(option);
			const auto color = std::cmp_equal(option_value, selected_option_value_) ? MainMenuOptionColorSelected : MainMenuOptionColorNormal;

			sf::Text text{*font, sf::String::fromUtf8(name.begin(), name.end()), MainMenuFontSize};
			text.setFillColor(color);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(1);
			text.setPosition({MainMenuBeginX, MainMenuBeginY + static_cast<float>(option_value) * MainMenuOptionHeight});

			window.draw(text);
		}
	}

	MainMenu::MainMenu(Game& game) noexcept
		: Scene{game},
		  font_id_{manager::InvalidAssetId},
		  sound_id_switch_option_{manager::InvalidAssetId},
		  music_id_{manager::InvalidAssetId},
		  selected_option_value_{std::to_underlying(option_type::START)} {}

	auto MainMenu::on_loaded() noexcept -> void
	{
		// using namespace game;
		// using namespace manager;

		font_id_ = manager::Font::load(map(game::Font::MAIN_MENU));
		sound_id_switch_option_ = manager::Sound::load(map(game::Sound::MENU_SWITCH_OPTION));
		music_id_ = manager::Music::load(map(game::Music::MAIN_MENU));
		{
			// 如果存在存档则为continue,否则为start
			selected_option_value_ = std::to_underlying(option_type::START);
		}
	}

	auto MainMenu::on_initialized() noexcept -> void
	{
		using namespace manager;

		Music::play(music_id_);
	}

	auto MainMenu::on_unloaded() noexcept -> void
	{
		using namespace manager;

		Music::stop(music_id_);

		Music::unload(music_id_);
		Sound::unload(sound_id_switch_option_);
		Font::unload(font_id_);
	}

	auto MainMenu::handle_event(const sf::Event& event) noexcept -> void
	{
		if (const auto& io = ImGui::GetIO();
			io.WantCaptureKeyboard || io.WantCaptureMouse)
		{
			return;
		}

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
