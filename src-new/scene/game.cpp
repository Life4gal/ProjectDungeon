// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/game.hpp>

#include <menu/pause.hpp>

// =========
// 管理器

#include <manager/asset.hpp>
#include <manager/event.hpp>

// =========
// 事件

// =========
// 监听器

// =========
// 访问

// =========
// 更新

// =========
// 渲染


// =========
// 依赖
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>

namespace pd::scene
{
	namespace
	{
		// 游戏音乐
		// TODO: 多个音乐
		// TODO: 暂停菜单是否需要切换音乐?
		constexpr std::string_view GameMusic = R"(.\media\musics\game.wav)";
	}

	auto Game::start_game() noexcept -> bool
	{
		// 进入地下城


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
		: music_id_{manager::InvalidAssetId},
		  frame_delta_{sf::seconds(1)},
		  total_elapsed_{sf::Time::Zero},
		  play_elapsed_{sf::Time::Zero},
		  is_paused_{false} {}

	Game::Game(Game&&) noexcept = default;

	auto Game::operator=(Game&&) noexcept -> Game& = default;

	Game::~Game() noexcept = default;

	auto Game::on_loaded() noexcept -> void
	{
		// 也许菜单创建可以延迟到第一次打开菜单时?不过目前先在这里创建好了
		pause_ = std::make_unique<menu::Pause>(is_paused_);

		// 加载资源
		music_id_ = manager::Music::load(GameMusic);

		// 订阅事件

		// 访问器初始化
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
		using manager::Font;
		using manager::Texture;
		using manager::Sound;
		using manager::Music;

		Music::stop(music_id_);

		// 销毁访问器


		// 退订事件


		// 卸载资源
		Music::unload(music_id_);
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
			pause_->handle_event(event);
		}
		else
		{
			if (const auto* kp = event.getIf<sf::Event::KeyPressed>())
			{
				if (kp->code == sf::Keyboard::Key::Escape)
				{
					is_paused_ = true;
				}
			}
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

		if (is_paused_)
		{
			pause_->update(delta);
		}
		else
		{
			//
		}
	}

	auto Game::render(sf::RenderWindow& window) noexcept -> void
	{
		// 

		if (is_paused_)
		{
			pause_->render(window);
		}

		//
	}
}
