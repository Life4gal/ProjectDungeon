// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/quit_game.hpp>

namespace pd::scene
{
	auto QuitGame::on_loaded() noexcept -> void {}

	auto QuitGame::on_initialized() noexcept -> void {}

	auto QuitGame::on_unloaded() noexcept -> void {}

	auto QuitGame::handle_event(const sf::Event& event) noexcept -> void {}

	auto QuitGame::update(const sf::Time delta) noexcept -> void {}

	auto QuitGame::render(sf::RenderWindow& window) noexcept -> void {}
}
