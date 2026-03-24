// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/game.hpp>

#include <game/menu_action.hpp>
#include <game/asset.hpp>

#include <components/asset_manager.hpp>

#include <systems/dispatcher.hpp>

#include <systems/asset_manager.hpp>
#include <systems/world.hpp>
#include <systems/physics_world.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>

namespace pd::scene
{
	auto Game::handle_event_pause(const sf::Event& event) noexcept -> void
	{
		using namespace systems;

		PROMETHEUS_PLATFORM_ASSUME(World::is_pause(registry_));

		const auto* kp = event.getIf<sf::Event::KeyPressed>();

		if (kp == nullptr)
		{
			return;
		}

		const auto action = game::map_action(kp->code);

		if (
			action != game::MenuAction::UP and
			action != game::MenuAction::DOWN and
			action != game::MenuAction::CONFIRM and
			action != game::MenuAction::CANCEL
		)
		{
			return;
		}

		if (action == game::MenuAction::UP or action == game::MenuAction::DOWN)
		{
			AssetManager::play_sound(registry_, sound_id_switch_option_);

			if (action == game::MenuAction::UP)
			{
				if (selected_pause_option_value_ == 0)
				{
					selected_pause_option_value_ = std::to_underlying(PauseMenuOption::COUNT) - 1;
				}
				else
				{
					selected_pause_option_value_ -= 1;
				}
			}
			else
			{
				if (selected_pause_option_value_ == std::to_underlying(PauseMenuOption::COUNT) - 1)
				{
					selected_pause_option_value_ = 0;
				}
				else
				{
					selected_pause_option_value_ += 1;
				}
			}
		}
		else if (action == game::MenuAction::CONFIRM)
		{
			if (selected_pause_option_value_ == std::to_underlying(PauseMenuOption::RESUME))
			{
				World::unpause(registry_);
			}
			else if (selected_pause_option_value_ == std::to_underlying(PauseMenuOption::OPTIONS))
			{
				//
			}
			else if (selected_pause_option_value_ == std::to_underlying(PauseMenuOption::QUIT))
			{
				// 切换到主菜单
				Dispatcher::scene_change(registry_, game::Scenes::MAIN_MENU);
			}
		}
		else if (action == game::MenuAction::CANCEL)
		{
			World::unpause(registry_);
		}
		else
		{
			PROMETHEUS_PLATFORM_UNREACHABLE();
		}
	}

	auto Game::start_game() noexcept -> bool
	{
		return false;
	}

	auto Game::load_game() noexcept -> bool
	{
		return false;
	}

	auto Game::restart_game() noexcept -> void
	{
		//
	}

	auto Game::on_loaded() noexcept -> void
	{
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

		font_id_ = AssetManager::load_font(registry_, game::map_font(game::Fonts::GAME));
		sound_id_switch_option_ = AssetManager::load_sound(registry_, game::map_sound(game::Sounds::MENU_SWITCH_OPTION));
		music_id_ = AssetManager::load_music(registry_, game::map_music(game::Musics::GAME));
		selected_pause_option_value_ = std::to_underlying(PauseMenuOption::RESUME);

		World::create(registry_);
		PhysicsWorld::create(registry_);
	}

	auto Game::on_initialized() noexcept -> void
	{
		using namespace systems;

		AssetManager::play_music(registry_, music_id_);

		// 这里可以检查是否存在存档?
		start_game();
	}

	auto Game::on_unloaded() noexcept -> void
	{
		using namespace systems;

		PhysicsWorld::destroy(registry_);
		World::destroy(registry_);

		AssetManager::stop_music(registry_, music_id_);
	}

	auto Game::handle_event(const sf::Event& event) noexcept -> void
	{
		using namespace systems;

		if (const auto& io = ImGui::GetIO();
			io.WantCaptureKeyboard || io.WantCaptureMouse)
		{
			return;
		}

		if (World::is_pause(registry_))
		{
			return handle_event_pause(event);
		}
	}

	auto Game::update(const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		//
	}

	auto Game::render(sf::RenderWindow& window) noexcept -> void
	{
		using namespace systems;

		//
	}
}
