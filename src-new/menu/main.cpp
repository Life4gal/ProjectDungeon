// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <menu/main.hpp>

#include <ranges>

#include <manager/resource.hpp>
#include <manager/audio_player.hpp>
#include <manager/event.hpp>
#include <manager/i18n.hpp>

#include <event/scene.hpp>

#include <SFML/Graphics.hpp>

namespace pd::menu
{
	namespace
	{
		// 主菜单选项开始位置
		constexpr float MainMenuBeginX = 290.f;
		constexpr float MainMenuBeginY = 300.f;
		// 主菜单选项字体
		constexpr std::string_view MainMenuFont = R"(C:\Windows\Fonts\msyh.ttc)";
		// 主菜单选项字体大小
		constexpr int MainMenuFontSize = 24;
		// 主菜单选项切换音效
		constexpr std::string_view MainMenuSoundSwitchOption = R"(.\media\sounds\menu_switch_option.ogg)";
		// 主菜单选项高度
		constexpr float MainMenuOptionHeight = static_cast<float>(MainMenuFontSize) * 1.5f;
		// 主菜单选项颜色
		constexpr sf::Color MainMenuOptionColorNormal{120, 120, 120, 255};
		constexpr sf::Color MainMenuOptionColorSelected{255, 255, 255, 255};

		// 主菜单各个选项对应Internationalization的key
		constexpr std::array MainMenuOptionI18NKeys
		{
				"MENU.MAIN.RANDOM_SEED",
				"MENU.MAIN.PLAYER_NAME",
				"MENU.MAIN.CONTINUE",
				"MENU.MAIN.START",
				"MENU.MAIN.OPTION",
				"MENU.MAIN.QUIT",
		};
	}

	auto Main::on_cursor_move(const int x, const int y) noexcept -> void
	{
		// todo
		std::ignore = x;
		std::ignore = y;
	}

	auto Main::on_cursor_click(const int x, const int y) noexcept -> void
	{
		// todo
		std::ignore = x;
		std::ignore = y;
	}

	auto Main::on_action(const Action action) noexcept -> void
	{
		using manager::AudioPlayer;
		using manager::Event;
		using event::scene::Switched;

		if (action == Action::UP or action == Action::DOWN)
		{
			AudioPlayer::play(sound_switch_option_);

			if (action == Action::UP)
			{
				if (selected_option_value_ == 0)
				{
					selected_option_value_ = std::to_underlying(Option::COUNT) - 1;
				}
				else
				{
					selected_option_value_ -= 1;
				}
			}
			else if (action == Action::DOWN)
			{
				if (selected_option_value_ == std::to_underlying(Option::COUNT) - 1)
				{
					selected_option_value_ = 0;
				}
				else
				{
					selected_option_value_ += 1;
				}
			}
		}
		else if (action == Action::CONFIRM)
		{
			if (const auto option = static_cast<Option>(selected_option_value_);
				option == Option::RANDOM_SEED)
			{
				// TODO: 输入种子
			}
			else if (option == Option::PLAYER_NAME)
			{
				// TODO: 输入玩家名称
			}
			else if (option == Option::CONTINUE)
			{
				Event::enqueue(Switched{.category = scene::Category::GAME_CONTINUE});
			}
			else if (option == Option::START)
			{
				Event::enqueue(Switched{.category = scene::Category::GAME});
			}
			else if (option == Option::OPTION)
			{
				// TODO: 打开选项菜单
			}
			else if (option == Option::QUIT)
			{
				Event::enqueue(Switched{.category = scene::Category::QUIT});
			}
		}
		else if (action == Action::CANCEL)
		{
			Event::enqueue(Switched{.category = scene::Category::QUIT});
		}
	}

	auto Main::on_update([[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		// 如果有动画效果也许有用
	}

	auto Main::on_render(sf::RenderWindow& window) noexcept -> void
	{
		using manager::Font;
		using manager::Internationalization;

		for (const auto option_value: std::views::iota(static_cast<std::underlying_type_t<Option>>(0), static_cast<std::underlying_type_t<Option>>(Option::COUNT)))
		{
			const auto& name = Internationalization::map(MainMenuOptionI18NKeys[option_value]);
			const auto color = std::cmp_equal(option_value, selected_option_value_) ? MainMenuOptionColorSelected : MainMenuOptionColorNormal;

			sf::Text text{*font_, sf::String::fromUtf8(name.begin(), name.end()), MainMenuFontSize};
			text.setFillColor(color);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(1);
			text.setPosition({MainMenuBeginX, MainMenuBeginY + static_cast<float>(option_value) * MainMenuOptionHeight});

			window.draw(text);
		}
	}

	Main::Main() noexcept
		: font_{manager::Font::load(MainMenuFont)},
		  sound_switch_option_{manager::Sound::load(MainMenuSoundSwitchOption)},
		  // 如果存在存档则为continue,否则为start
		  selected_option_value_{std::to_underlying(Option::START)}
	{
		//
	}

	// Main::~Main() noexcept = default;
}
