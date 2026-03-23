// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <game/game.hpp>

#include <components/game_config.hpp>

#include <scene/quit_game.hpp>
#include <scene/main_menu.hpp>

#include <external/imgui-SFML.hpp>
#include <imgui.h>

namespace pd
{
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
		// IMGUI
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
		// 注册全局组件
		{
			// 游戏实例
			// registry_.ctx().emplace<components::Game>(std::cref(*this));

			// 游戏配置
			registry_.ctx().emplace<components::GameConfig>();
		}
		// 创建场景
		{
			// 退出场景(占位)
			{
				auto scene = std::make_unique<scene::QuitGame>(registry_);
				quit_game_scene_ = scenes_.emplace_back(std::move(scene)).get();
			}

			// 主菜单
			{
				auto scene = std::make_unique<scene::MainMenu>(registry_);
				current_scene_ = scenes_.emplace_back(std::move(scene)).get();
				current_scene_->on_loaded();
				current_scene_->on_initialized();
			}
		}
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
					// 卸载当前场景
					current_scene_->on_unloaded();
					// 切换到退出场景
					current_scene_ = quit_game_scene_;

					// 关闭窗口
					window_.close();
				}
				else
				{
					// 当前场景处理事件
					current_scene_->handle_event(e);
				}
			}

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
