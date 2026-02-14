// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main.hpp>

#include <print>

#include <imgui.h>

namespace pd::scene
{
	auto Main::start_game() noexcept -> void
	{
		// todo
	}

	Main::Main(const std::reference_wrapper<entt::registry> global_registry) noexcept
		: Scene{global_registry} {}

	auto Main::on_loaded() noexcept -> void
	{
		std::println("MainScene::on_loaded");

		world_system_.on_loaded(scene_registry_);
		blueprint_system_.on_loaded(scene_registry_);
		asset_system_.on_loaded(scene_registry_);
		flip_animation_system_.on_loaded(scene_registry_);

		debug_system_.on_loaded(scene_registry_);
	}

	auto Main::on_initialized() noexcept -> void
	{
		std::println("MainScene::on_initialized");

		world_system_.on_initialized(scene_registry_);
		blueprint_system_.on_initialized(scene_registry_);
		asset_system_.on_initialized(scene_registry_);
		flip_animation_system_.on_initialized(scene_registry_);

		debug_system_.on_initialized(scene_registry_);
	}

	auto Main::on_unloaded() noexcept -> void
	{
		std::println("MainScene::on_unloaded");

		// 与on_loaded相反顺序执行
		debug_system_.on_unloaded(scene_registry_);

		flip_animation_system_.on_unloaded(scene_registry_);
		asset_system_.on_unloaded(scene_registry_);
		blueprint_system_.on_unloaded(scene_registry_);
		world_system_.on_unloaded(scene_registry_);

		// todo: 更新全局统计数据等信息?
	}

	auto Main::handle_event(const sf::Event& event) noexcept -> void
	{
		const auto& io = ImGui::GetIO();
		const auto want_capture_keyboard = io.WantCaptureKeyboard;
		const auto want_capture_mouse = io.WantCaptureMouse;

		if (want_capture_keyboard or want_capture_mouse)
		{
			return;
		}

		// todo: 处理事件
	}

	auto Main::update(const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		world_system_.update(scene_registry_, delta);
		blueprint_system_.update(scene_registry_, delta);
		asset_system_.update(scene_registry_, delta);
		flip_animation_system_.update(scene_registry_, delta);

		debug_system_.update(scene_registry_, delta);
	}

	auto Main::render(sf::RenderWindow& window) noexcept -> void
	{
		using namespace systems;

		world_system_.render(scene_registry_, window);
		blueprint_system_.render(scene_registry_, window);
		asset_system_.render(scene_registry_, window);
		flip_animation_system_.render(scene_registry_, window);

		debug_system_.render(scene_registry_, window);
	}
}
