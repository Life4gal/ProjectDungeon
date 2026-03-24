// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

#include <SFML/Window/Keyboard.hpp>

namespace pd::game
{
	enum class MenuAction : std::uint8_t
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		CONFIRM,
		CANCEL,

		COUNT,

		NONE,
	};

	class MenuActionMap final
	{
	public:
		// FIXME: 可配置?
		constexpr static std::array/*<sf::Keyboard::Key, std::to_underlying(MenuAction::COUNT)>*/ action_keys
		{
				// UP
				sf::Keyboard::Key::Up,
				// DOWN
				sf::Keyboard::Key::Down,
				// LEFT
				sf::Keyboard::Key::Left,
				// RIGHT
				sf::Keyboard::Key::Right,
				// CONFIRM
				sf::Keyboard::Key::Enter,
				// CANCEL
				sf::Keyboard::Key::Escape,
		};

		[[nodiscard]] constexpr static auto get(const sf::Keyboard::Key& key) noexcept -> MenuAction
		{
			for (std::size_t i = 0; i < std::to_underlying(MenuAction::COUNT); ++i)
			{
				if (key == action_keys[i])
				{
					return static_cast<MenuAction>(i);
				}
			}
			return MenuAction::NONE;
		}
	};
}
