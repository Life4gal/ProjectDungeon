// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main_menu.hpp>

#include <manager/asset.hpp>

#include <menu/main.hpp>

#include <imgui.h>

namespace pd::scene
{
	namespace
	{
		// 主菜单音乐
		constexpr std::string_view MainMenuMusic = R"(.\media\musics\main_menu.wav)";
	}

	MainMenu::MainMenu() noexcept
		: music_id_{manager::InvalidAssetId} {}

	MainMenu::MainMenu(MainMenu&&) noexcept = default;

	auto MainMenu::operator=(MainMenu&&) noexcept -> MainMenu& = default;

	MainMenu::~MainMenu() noexcept = default;

	auto MainMenu::on_loaded() noexcept -> void
	{
		main_ = std::make_unique<menu::Main>();

		music_id_ = manager::Music::load(MainMenuMusic);
	}

	auto MainMenu::on_initialized() noexcept -> void
	{
		using manager::Music;

		Music::play(music_id_);
	}

	auto MainMenu::on_unloaded() noexcept -> void
	{
		using manager::Music;

		Music::stop(music_id_);
		Music::unload(music_id_);

		main_.reset();
	}

	auto MainMenu::handle_event(const sf::Event& event) noexcept -> void
	{
		if (const auto& io = ImGui::GetIO();
			io.WantCaptureKeyboard || io.WantCaptureMouse)
		{
			return;
		}

		main_->handle_event(event);
	}

	auto MainMenu::update(const sf::Time delta) noexcept -> void
	{
		main_->update(delta);
	}

	auto MainMenu::render(sf::RenderWindow& window) noexcept -> void
	{
		main_->render(window);
	}
}
