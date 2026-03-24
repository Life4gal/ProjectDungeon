// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <game/game.hpp>

#include <print>

#include <events/game.hpp>

#include <game/scene.hpp>

#include <scene/main_menu.hpp>

#include <external/imgui-SFML.hpp>
#include <imgui.h>

namespace pd
{
	auto Game::on_request_scene_change(const events::RequestSceneChange& event) noexcept -> void
	{
		if (current_scene_)
		{
			current_scene_->on_unloaded();
		}

		// 切换到退出场景时直接关闭窗口
		if (event.to == game::Scenes::QUIT)
		{
			std::println("QUIT...");

			window_.close();

			return;
		}

		if (event.to == game::Scenes::MAIN_MENU)
		{
			std::println("MAIN MAIN...");

			current_scene_ = std::make_unique<scene::MainMenu>(*this);
			current_scene_->on_loaded();
			current_scene_->on_initialized();

			return;
		}

		if (event.to == game::Scenes::GAME)
		{
			std::println("GAME...");

			// todo
			return;
		}
	}

	Game::Game(
		const unsigned window_width,
		const unsigned window_height,
		const std::string_view title,
		const bool fullscreen
	) noexcept
		: window_
		  {
				  sf::VideoMode{{window_width, window_height}},
				  sf::String::fromUtf8(title.begin(), title.end()),
				  sf::Style::Default,
				  fullscreen ? sf::State::Fullscreen : sf::State::Windowed
		  },
		  current_scene_{nullptr}
	{
		// IMGUI初始化
		{
			// const auto init = ImGui::SFML::Init(window);
			// assert(init);

			const auto init = ImGui::SFML::Init(window_, false);
			assert(init);

			auto* fonts = ImGui::GetIO().Fonts;
			fonts->AddFontFromFileTTF(
				R"(C:\Windows\Fonts\msyh.ttc)",
				16.f,
				nullptr,
				fonts->GetGlyphRangesChineseSimplifiedCommon()
			);

			const auto update = ImGui::SFML::UpdateFontTexture();
			assert(update);
		}
		// 订阅场景切换事件
		dispatcher_.subscribe<events::RequestSceneChange, &Game::on_request_scene_change>(*this);

		// 切换场景
		// 注意是trigger,因为我们必须马上创建所需场景
		dispatcher_.trigger(events::RequestSceneChange{.to = game::Scenes::MAIN_MENU});
	}

	Game::~Game() noexcept
	{
		ImGui::SFML::Shutdown();
	}

	auto Game::run() noexcept -> void
	{
		// 在这里重置绝对计时器,其计时从此刻开始
		absolute_clock_.restart();

		while (window_.isOpen())
		{
			while (const auto event = window_.pollEvent())
			{
				const auto e = *event;

				ImGui::SFML::ProcessEvent(window_, e);

				if (event->is<sf::Event::Closed>())
				{
					// 在这里使用enqueue而不是trigger,我们保证退出前依然会处理完所有事件
					dispatcher_.enqueue(events::RequestSceneChange{.to = game::Scenes::QUIT});
				}
				else
				{
					// 当前场景处理事件
					current_scene_->handle_event(e);
				}
			}

			// 处理事件
			dispatcher_.update();

			const auto delta = clock_.restart();

			// 更新
			ImGui::SFML::Update(window_, delta);
			// 当前场景更新
			current_scene_->update(delta);

			window_.clear({35, 35, 35});

			// 当前场景绘制
			current_scene_->render(window_);
			ImGui::SFML::Render(window_);

			window_.display();
		}
	}

	auto Game::game_config() noexcept -> GameConfig&
	{
		return config_;
	}

	auto Game::game_config() const noexcept -> const GameConfig&
	{
		return config_;
	}

	auto Game::dispatcher() noexcept -> Dispatcher&
	{
		return dispatcher_;
	}

	auto Game::window_size() const noexcept -> sf::Vector2u
	{
		return window_.getSize();
	}

	auto Game::window_width() const noexcept -> unsigned
	{
		return window_size().x;
	}

	auto Game::window_height() const noexcept -> unsigned
	{
		return window_size().y;
	}

	auto Game::time() const noexcept -> sf::Time
	{
		return absolute_clock_.getElapsedTime();
	}
}
