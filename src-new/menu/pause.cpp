// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <menu/pause.hpp>

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
		// 暂停菜单选项开始位置
		constexpr float PauseMenuBeginX = 400.f;
		constexpr float PauseMenuBeginY = 300.f;
		// 暂停菜单选项字体
		constexpr std::string_view PauseMenuFont = R"(C:\Windows\Fonts\msyh.ttc)";
		// 暂停菜单选项字体大小
		constexpr int PauseMenuFontSize = 24;
		// 暂停菜单选项切换音效
		constexpr std::string_view PauseMenuSoundSwitchOption = R"(.\media\sounds\menu_switch_option.ogg)";
		// 暂停菜单选项高度
		constexpr float PauseMenuOptionHeight = static_cast<float>(PauseMenuFontSize) * 1.5f;
		// 暂停菜单选项颜色
		constexpr sf::Color PauseMenuOptionColorNormal{120, 120, 120, 255};
		constexpr sf::Color PauseMenuOptionColorSelected{255, 255, 255, 255};

		// 暂停菜单各个选项对应Internationalization的key
		constexpr std::array PauseMenuOptionI18NKeys
		{
				"MENU.PAUSE.RESUME",
				"MENU.PAUSE.OPTION",
				"MENU.PAUSE.TO_MAIN_MENU",
				"MENU.PAUSE.TO_DESKTOP",
		};
	}


	auto Pause::on_cursor_move(const int x, const int y) noexcept -> void
	{
		// todo
		std::ignore = x;
		std::ignore = y;
	}

	auto Pause::on_cursor_click(const int x, const int y) noexcept -> void
	{
		// todo
		std::ignore = x;
		std::ignore = y;
	}

	auto Pause::on_action(const Action action) noexcept -> void
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
				option == Option::RESUME)
			{
				pause_ref_.get() = false;
			}
			else if (option == Option::OPTION)
			{
				// TODO: 打开选项菜单
			}
			else if (option == Option::TO_MAIN_MENU)
			{
				Event::enqueue(Switched{.category = scene::Category::MAIN_MENU});
			}
			else if (option == Option::TO_DESKTOP)
			{
				Event::enqueue(Switched{.category = scene::Category::QUIT});
			}
		}
		else if (action == Action::CANCEL)
		{
			pause_ref_.get() = false;
		}
	}

	auto Pause::on_update(sf::Time delta) noexcept -> void
	{
		// 如果有动画效果也许有用
	}

	auto Pause::on_render(sf::RenderWindow& window) noexcept -> void
	{
		using manager::Font;
		using manager::Internationalization;

		for (const auto option_value: std::views::iota(static_cast<std::underlying_type_t<Option>>(0), static_cast<std::underlying_type_t<Option>>(Option::COUNT)))
		{
			const auto& name = Internationalization::map(PauseMenuOptionI18NKeys[option_value]);
			const auto color = std::cmp_equal(option_value, selected_option_value_) ? PauseMenuOptionColorSelected : PauseMenuOptionColorNormal;

			sf::Text text{*font_, sf::String::fromUtf8(name.begin(), name.end()), PauseMenuFontSize};
			text.setFillColor(color);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(1);
			text.setPosition({PauseMenuBeginX, PauseMenuBeginY + static_cast<float>(option_value) * PauseMenuOptionHeight});

			window.draw(text);
		}
	}

	Pause::Pause(bool& pause) noexcept
		: font_{manager::Font::load(PauseMenuFont)},
		  sound_switch_option_{manager::Sound::load(PauseMenuSoundSwitchOption)},
		  selected_option_value_{std::to_underlying(Option::RESUME)},
		  pause_ref_{pause}
	{
		//
	}

	// Pause::~Pause() noexcept = default;
}
