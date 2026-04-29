// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <game.hpp>

#include <manager/event.hpp>

#include <var/window.hpp>

#include <scene/main_menu.hpp>
#include <scene/game.hpp>

#include <prometheus/meta/enumeration.hpp>
#include <prometheus/platform/os.hpp>
#include <spdlog/spdlog.h>
#include <external/imgui-SFML.hpp>
#include <imgui.h>

namespace pd
{
	auto Game::on_window_resized(const event::window::Resized& event) noexcept -> void
	{
		var::window_width = event.width;
		var::window_height = event.height;
	}

	auto Game::on_scene_switched(const event::scene::Switched& event) noexcept -> void
	{
		SPDLOG_INFO("SceneSwitch --> {}", prometheus::meta::name_of(event.category));
		pending_scene_ = event.category;

		if (current_scene_)
		{
			current_scene_->on_unloaded();
		}

		switch (event.category)
		{
			case scene::Category::MAIN_MENU:
			case scene::Category::GAME:
			case scene::Category::GAME_CONTINUE:
			{
				//
				break;
			}
			case scene::Category::QUIT:
			{
				window_.close();
				break;
			}
			case scene::Category::COUNT:
			{
				PROMETHEUS_PLATFORM_UNREACHABLE();
			}
		}
	}

	Game::Game(const unsigned window_width, const unsigned window_height, const std::string_view title, const bool fullscreen) noexcept
		: window_
		  {
				  sf::VideoMode{{window_width, window_height}},
				  sf::String::fromUtf8(title.begin(), title.end()),
				  sf::Style::Default,
				  fullscreen ? sf::State::Fullscreen : sf::State::Windowed
		  },
		  current_scene_{nullptr},
		  pending_scene_{std::nullopt}
	{
		// IMGUI初始化
		{
			// const auto init = ImGui::SFML::Init(window);
			// assert(init);

			const auto init = ImGui::SFML::Init(window_, false);
			assert(init);

			auto* fonts = ImGui::GetIO().Fonts;
			fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\msyh.ttc)", 16.f, nullptr, fonts->GetGlyphRangesChineseSimplifiedCommon());

			const auto update = ImGui::SFML::UpdateFontTexture();
			assert(update);
		}

		// 订阅事件
		manager::Event::subscribe<event::window::Resized, &Game::on_window_resized>();
		manager::Event::subscribe<event::scene::Switched, &Game::on_scene_switched>(this);

		// 设置Var
		var::window_width = window_width;
		var::window_height = window_height;

		// 创建场景
		current_scene_ = std::make_unique<scene::MainMenu>();
		current_scene_->on_loaded();
		current_scene_->on_initialized();
	}

	Game::~Game() noexcept
	{
		// 退订事件
		manager::Event::unsubscribe<event::window::Resized, &Game::on_window_resized>();
		manager::Event::unsubscribe<event::scene::Switched, &Game::on_scene_switched>(this);

		ImGui::SFML::Shutdown();
	}

	auto Game::run() noexcept -> void
	{
		using manager::Event;

		while (window_.isOpen())
		{
			while (const auto event = window_.pollEvent())
			{
				const auto e = *event;

				ImGui::SFML::ProcessEvent(window_, e);

				if (event->is<sf::Event::Closed>())
				{
					// 在这里使用enqueue而不是trigger,我们保证退出前依然会处理完所有事件
					Event::enqueue(event::scene::Switched{.category = scene::Category::QUIT});
					continue;
				}

				if (const auto* r = event->getIf<sf::Event::Resized>())
				{
					Event::enqueue(event::window::Resized{.width = r->size.x, .height = r->size.y});
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
				const auto category = *pending_scene_;
				pending_scene_.reset();

				if (category != scene::Category::QUIT)
				{
					if (category == scene::Category::MAIN_MENU)
					{
						current_scene_ = std::make_unique<scene::MainMenu>();
					}
					else if (category == scene::Category::GAME or category == scene::Category::GAME_CONTINUE)
					{
						current_scene_ = std::make_unique<scene::Game>();
					}
					else
					{
						PROMETHEUS_PLATFORM_UNREACHABLE();
					}

					current_scene_->on_loaded();
					current_scene_->on_initialized();
				}
			}
		}
	}
}
