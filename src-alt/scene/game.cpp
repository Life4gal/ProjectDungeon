// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/game.hpp>

#include <game/constants_map.hpp>

#include <events/scene.hpp>
#include <events/dungeon.hpp>

#include <manager/event.hpp>
#include <manager/asset.hpp>

#include <systems/world.hpp>
#include <systems/physics_world.hpp>

#include <systems/dungeon.hpp>
#include <systems/floor.hpp>
#include <systems/room.hpp>
#include <systems/door.hpp>
#include <systems/chest.hpp>
#include <systems/tile.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>

namespace pd::scene
{
	auto Game::handle_event_pause(const sf::Event& event) noexcept -> void
	{
		using namespace game;
		using namespace manager;
		using namespace systems;

		PROMETHEUS_PLATFORM_ASSUME(World::is_pause(registry_));

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
			if (selected_option_value_ == std::to_underlying(option_type::RESUME))
			{
				World::unpause(registry_);
			}
			else if (selected_option_value_ == std::to_underlying(option_type::OPTIONS))
			{
				//
			}
			else if (selected_option_value_ == std::to_underlying(option_type::QUIT))
			{
				// 切换到主菜单
				Event::enqueue(events::SceneChanged{.to = Type::MAIN_MENU});
			}
		}
		else if (action == MenuAction::CANCEL)
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
		using namespace manager;
		using namespace events;

		// 进入地下城
		Event::enqueue(dungeon::Go{.level = 1, .x = game::FloorStartRoomX, .y = game::FloorStartRoomY});

		return true;
	}

	auto Game::load_game() noexcept -> bool
	{
		return false;
	}

	auto Game::restart_game() noexcept -> void
	{
		//
	}

	Game::Game(pd::Game& game) noexcept
		: Scene{game},
		  font_id_hud_{manager::InvalidAssetId},
		  font_id_pause_menu_{manager::InvalidAssetId},
		  texture_id_wall_{manager::InvalidAssetId},
		  texture_id_floor_{manager::InvalidAssetId},
		  sound_id_switch_option_{manager::InvalidAssetId},
		  music_id_{manager::InvalidAssetId},
		  selected_option_value_{std::to_underlying(option_type::RESUME)} {}

	auto Game::on_loaded() noexcept -> void
	{
		// using namespace game;
		// using namespace manager;
		using namespace systems;

		// 加载资源
		font_id_hud_ = manager::Font::load(map(game::Font::GAME_HUD));
		font_id_pause_menu_ = manager::Font::load(map(game::Font::GAME_PAUSE_MENU));
		texture_id_wall_ = manager::Texture::load(map(game::Texture::WALL));
		texture_id_floor_ = manager::Texture::load(map(game::Texture::FLOOR));
		sound_id_switch_option_ = manager::Sound::load(map(game::Sound::MENU_SWITCH_OPTION));
		music_id_ = manager::Music::load(map(game::Music::GAME));
		selected_option_value_ = std::to_underlying(option_type::RESUME);

		// 创建系统
		World::create(registry_);
		PhysicsWorld::create(registry_);

		Dungeon::create(registry_);
		Floor::create(registry_);
		Room::create(registry_);

		// 订阅事件
		Dungeon::subscribe_events(registry_);
		Floor::subscribe_events(registry_);
		Room::subscribe_events(registry_);
		Door::subscribe_events(registry_);
		Chest::subscribe_events(registry_);
		Tile::subscribe_events(registry_);
	}

	auto Game::on_initialized() noexcept -> void
	{
		using namespace manager;

		Music::play(music_id_);

		// 这里可以检查是否存在存档?
		start_game();
	}

	auto Game::on_unloaded() noexcept -> void
	{
		using namespace manager;
		using namespace systems;

		Music::stop(music_id_);

		// 退订事件
		Tile::unsubscribe_events(registry_);
		Chest::unsubscribe_events(registry_);
		Door::unsubscribe_events(registry_);
		Room::unsubscribe_events(registry_);
		Floor::unsubscribe_events(registry_);
		Dungeon::unsubscribe_events(registry_);

		// 销毁系统
		Room::destroy(registry_);
		Floor::destroy(registry_);
		Dungeon::destroy(registry_);

		PhysicsWorld::destroy(registry_);
		World::destroy(registry_);

		// 卸载资源
		Sound::unload(sound_id_switch_option_);
		Texture::unload(texture_id_floor_);
		Texture::unload(texture_id_wall_);
		Font::unload(font_id_pause_menu_);
		Font::unload(font_id_hud_);
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
