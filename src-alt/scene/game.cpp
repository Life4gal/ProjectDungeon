// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/game.hpp>

#include <game/constants_map.hpp>

// =========
// 管理器
#include <manager/event.hpp>
#include <manager/asset.hpp>

// =========
// 事件
#include <events/scene.hpp>
#include <events/dungeon.hpp>

// =========
// 监听器
#include <listener/door.hpp>
#include <listener/chest.hpp>
#include <listener/room.hpp>
#include <listener/floor.hpp>
#include <listener/dungeon.hpp>

// =========
// 更新

// =========
// 渲染
#include <render/renderable.hpp>

// =========
// 依赖
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <imgui.h>

namespace pd::scene
{
	b2WorldId Game::physics_world_id = b2_nullWorldId;

	auto Game::handle_event_pause(const sf::Event& event) noexcept -> void
	{
		using namespace game;
		using namespace manager;

		PROMETHEUS_PLATFORM_ASSUME(is_paused_);

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
				is_paused_ = false;
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
			is_paused_ = false;
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

	Game::Game() noexcept
		:
		font_id_hud_{manager::InvalidAssetId},
		font_id_pause_menu_{manager::InvalidAssetId},
		texture_id_wall_{manager::InvalidAssetId},
		texture_id_floor_{manager::InvalidAssetId},
		sound_id_switch_option_{manager::InvalidAssetId},
		music_id_{manager::InvalidAssetId},
		selected_option_value_{std::to_underlying(option_type::RESUME)},
		frame_delta_{sf::seconds(1)},
		total_elapsed_{sf::Time::Zero},
		play_elapsed_{sf::Time::Zero},
		is_paused_{false} {}

	auto Game::on_loaded() noexcept -> void
	{
		// 加载资源
		font_id_hud_ = manager::Font::load(map(game::Font::GAME_HUD));
		font_id_pause_menu_ = manager::Font::load(map(game::Font::GAME_PAUSE_MENU));
		texture_id_wall_ = manager::Texture::load(map(game::Texture::WALL));
		texture_id_floor_ = manager::Texture::load(map(game::Texture::FLOOR));
		sound_id_switch_option_ = manager::Sound::load(map(game::Sound::MENU_SWITCH_OPTION));
		music_id_ = manager::Music::load(map(game::Music::GAME));
		selected_option_value_ = std::to_underlying(option_type::RESUME);

		// 创建物理世界
		{
			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NULL(physics_world_id));

			auto def = b2DefaultWorldDef();
			// 无重力世界(俯视角)
			def.gravity = b2Vec2_zero;
			physics_world_id = b2CreateWorld(&def);
		}

		// 订阅事件
		listener::Door::subscribe(registry_);
		listener::Chest::subscribe(registry_);
		listener::Room::subscribe(registry_);
		listener::Floor::subscribe(registry_);
		listener::Dungeon::subscribe(registry_);
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
		manager::Music::stop(music_id_);

		// 退订事件
		listener::Door::subscribe(registry_);
		listener::Chest::subscribe(registry_);
		listener::Room::subscribe(registry_);
		listener::Floor::subscribe(registry_);
		listener::Dungeon::subscribe(registry_);

		// 销毁物理世界
		{
			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(physics_world_id));

			b2DestroyWorld(physics_world_id);
			physics_world_id = b2_nullWorldId;
		}

		// 卸载资源
		manager::Sound::unload(sound_id_switch_option_);
		manager::Texture::unload(texture_id_floor_);
		manager::Texture::unload(texture_id_wall_);
		manager::Font::unload(font_id_pause_menu_);
		manager::Font::unload(font_id_hud_);
	}

	auto Game::handle_event(const sf::Event& event) noexcept -> void
	{
		if (const auto& io = ImGui::GetIO();
			io.WantCaptureKeyboard || io.WantCaptureMouse)
		{
			return;
		}

		if (is_paused_)
		{
			return handle_event_pause(event);
		}
	}

	auto Game::update(const sf::Time delta) noexcept -> void
	{
		frame_delta_ = delta;
		total_elapsed_ += delta;
		if (not is_paused_)
		{
			play_elapsed_ += delta;
		}

		// todo
	}

	auto Game::render(sf::RenderWindow& window) noexcept -> void
	{
		render::renderable(registry_, window);
	}
}
