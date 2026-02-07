// =====================================
// SFML
#include <SFML/Graphics.hpp>

// =====================================
// IMGUI

#include <imgui.h>
#include <pd/external/imgui-SFML.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

auto main() noexcept -> int
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	constexpr int window_width = 1920;
	constexpr int window_height = 1080;

	const sf::VideoMode video_mode{{window_width, window_height}};
	sf::RenderWindow window{video_mode, "TD"};
	window.setFramerateLimit(60);
	sf::Clock delta_clock{};

	// IMGUI
	{
		// const auto init = ImGui::SFML::Init(window);
		// assert(init);

		const auto init = ImGui::SFML::Init(window, false);
		assert(init);

		auto* fonts = ImGui::GetIO().Fonts;
		fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\msyh.ttc)", 16.f, nullptr, fonts->GetGlyphRangesChineseSimplifiedCommon());

		const auto update = ImGui::SFML::UpdateFontTexture();
		assert(update);
	}

	// 初始化场景

	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);

			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else
			{
				// 处理事件
				// todo
			}
		}

		const auto delta = delta_clock.restart();

		// 更新游戏状态
		ImGui::SFML::Update(window, delta);

		// 更新场景
		// todo

		ImGui::ShowDemoWindow();

		// 绘制
		window.clear({35, 35, 35});

		// 绘制场景
		// todo
		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
}
