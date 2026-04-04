// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <game/game.hpp>

#include <print>

#include <game/constants_map.hpp>

#include <scene/main_menu.hpp>
#include <scene/game.hpp>

#include <manager/random.hpp>
#include <manager/asset.hpp>
#include <manager/event.hpp>
#include <manager/game_config.hpp>

#include <external/imgui-SFML.hpp>
#include <imgui.h>

namespace pd
{
	auto Game::on_window_resized(const events::WindowResized& event) noexcept -> void
	{
		using namespace manager;

		std::ignore = this;

		// 写入GameConfig
		GameConfig::set_window_width(event.width);
		GameConfig::set_window_height(event.height);
	}

	auto Game::on_scene_changed(const events::SceneChanged& event) noexcept -> void
	{
		using namespace game;
		using namespace manager;

		if (current_scene_)
		{
			current_scene_->on_unloaded();
		}

		// 切换到退出场景时直接关闭窗口
		if (event.to == scene::Type::QUIT)
		{
			std::println("QUIT...");

			window_.close();

			return;
		}

		if (event.to == scene::Type::MAIN_MENU)
		{
			std::println("MAIN MAIN...");

			// current_scene_ = std::make_unique<scene::MainMenu>(*this);
			// current_scene_->on_loaded();
			// current_scene_->on_initialized();
			pending_scene_ = scene::Type::MAIN_MENU;

			return;
		}

		if (event.to == scene::Type::GAME)
		{
			std::println("GAME...");

			// current_scene_ = std::make_unique<scene::Game>(*this);
			// current_scene_->on_loaded();
			// current_scene_->on_initialized();
			pending_scene_ = scene::Type::GAME;

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
		using namespace game;
		using namespace manager;

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

		// GameConfig
		GameConfig::set_window_width(window_width);
		GameConfig::set_window_height(window_height);

		// 订阅场景切换事件
		Event::subscribe<events::SceneChanged, &Game::on_scene_changed>(*this);

		Event::instance().sink<events::dungeon::Go>().connect<[](const events::dungeon::Go& go) {}>();

		// 切换场景
		// 注意是trigger,因为我们必须马上创建所需场景
		// Event::trigger(events::SceneChanged{.to = scene::Type::MAIN_MENU});
		current_scene_ = std::make_unique<scene::MainMenu>(*this);
		current_scene_->on_loaded();
		current_scene_->on_initialized();
	}

	Game::~Game() noexcept
	{
		ImGui::SFML::Shutdown();
	}

	auto Game::run() noexcept -> void
	{
		using namespace game;
		using namespace manager;

		while (window_.isOpen())
		{
			while (const auto event = window_.pollEvent())
			{
				const auto e = *event;

				ImGui::SFML::ProcessEvent(window_, e);

				if (event->is<sf::Event::Closed>())
				{
					// 在这里使用enqueue而不是trigger,我们保证退出前依然会处理完所有事件
					Event::enqueue(events::SceneChanged{.to = scene::Type::QUIT});
					continue;
				}

				if (const auto* r = event->getIf<sf::Event::Resized>())
				{
					// 对于窗口大小调整事件,我们不直接修改GameConfig,而是入列一个事件,这样其他地方也能处理该事件
					Event::enqueue(events::WindowResized{.width = r->size.x, .height = r->size.y});
					continue;
				}

				// 当前场景处理事件
				current_scene_->handle_event(e);
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

			// 我们将所有队列中的事件延后到该帧最后处理,以保证该帧能正常更新/渲染
			Event::update();

			// 创建场景实例(如果需要)
			if (pending_scene_)
			{
				if (const auto type = *pending_scene_;
					type == scene::Type::MAIN_MENU)
				{
					current_scene_ = std::make_unique<scene::MainMenu>(*this);
				}
				else if (type == scene::Type::GAME)
				{
					current_scene_ = std::make_unique<scene::Game>(*this);
				}

				current_scene_->on_loaded();
				current_scene_->on_initialized();

				pending_scene_.reset();
			}
		}
	}
}
