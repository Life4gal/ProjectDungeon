// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <menu/menu.hpp>

#include <array>

#include <var/key_action_mapper.hpp>

#include <SFML/Window/Event.hpp>

namespace pd::menu
{
	ActionMapper::ActionMapper() noexcept = default;

	ActionMapper::~ActionMapper() noexcept = default;

	auto ActionMapper::map(const sf::Keyboard::Key key) const noexcept -> Action
	{
		using sf::Keyboard::Key;

		constexpr std::array keys
		{
				// UP
				Key::Up,
				// DOWN
				Key::Down,
				// LEFT
				Key::Left,
				// RIGHT
				Key::Right,
				// CONFIRM
				Key::Enter,
				// CANCEL
				Key::Escape,
		};

		for (std::size_t i = 0; i < std::to_underlying(Action::COUNT); ++i)
		{
			if (key == keys[i])
			{
				return static_cast<Action>(i);
			}
		}

		return Action::NONE;
	}

	Menu::Menu() noexcept = default;

	Menu::~Menu() noexcept = default;

	auto Menu::handle_event(const sf::Event& event) noexcept -> bool
	{
		if (const auto* mm = event.getIf<sf::Event::MouseMoved>())
		{
			on_cursor_move(mm->position.x, mm->position.y);
			return true;
		}

		if (const auto* mp = event.getIf<sf::Event::MouseButtonPressed>())
		{
			on_cursor_click(mp->position.x, mp->position.y);
			return true;
		}

		if (const auto* kp = event.getIf<sf::Event::KeyPressed>())
		{
			if (const auto action = var::key_action_mapper->map(kp->code);
				action != Action::NONE)
			{
				on_action(action);
				return true;
			}
		}

		return false;
	}

	auto Menu::update(const sf::Time delta) noexcept -> void
	{
		on_update(delta);
	}

	auto Menu::render(sf::RenderWindow& window) noexcept -> void
	{
		on_render(window);
	}
}
