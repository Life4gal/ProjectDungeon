// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <game/game.hpp>

#include <game/main_scene.hpp>

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
		// 创建场景
		{
			auto scene = std::make_unique<MainScene>(registry_);

			current_scene_ = scenes_.emplace_back(std::move(scene)).get();
		}
	}

	Game::~Game() noexcept
	{
		ImGui::SFML::Shutdown();
	}

	auto Game::run() noexcept -> void
	{
		while (window_.isOpen())
		{
			while (const auto event = window_.pollEvent())
			{
				const auto e = *event;

				ImGui::SFML::ProcessEvent(window_, e);

				if (event->is<sf::Event::Closed>())
				{
					window_.close();
				}
				else
				{
					current_scene_->handle_event(e);
				}
			}

			const auto delta = clock_.restart();

			// 更新
			ImGui::SFML::Update(window_, delta);
			current_scene_->update(delta);

			window_.clear({35, 35, 35});

			// 绘制
			current_scene_->render(window_);
			ImGui::SFML::Render(window_);

			window_.display();
		}
	}
}
