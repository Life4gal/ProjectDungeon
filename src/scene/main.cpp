// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main.hpp>

#include <print>

#include <components/world.hpp>

#include <game/defines.hpp>

#include <box2d/box2d.h>

#include <SFML/Graphics.hpp>

#include <imgui.h>

namespace pd::scene
{
	auto Main::create_physics_world() noexcept -> void
	{
		auto def = b2DefaultWorldDef();
		// 无重力世界
		def.gravity = b2Vec2_zero;

		physics_world_id_ = b2CreateWorld(&def);
	}

	auto Main::register_components() noexcept -> void
	{
		scene_registry_.ctx().emplace<components::FrameDelta>(sf::seconds(1));
		scene_registry_.ctx().emplace<components::SceneElapsedTime>(sf::Time::Zero);
		scene_registry_.ctx().emplace<components::SceneRealElapsedTime>(sf::Time::Zero);
	}

	auto Main::start_game() noexcept -> void
	{
		// todo
	}

	Main::Main(const std::reference_wrapper<entt::registry> global_registry) noexcept
		: Scene{global_registry},
		  physics_world_id_{b2_nullWorldId} {}

	Main::~Main() noexcept
	{
		b2DestroyWorld(physics_world_id_);
	}

	auto Main::on_loaded() noexcept -> void
	{
		std::println("MainScene::on_loaded");

		// 创建物理世界
		create_physics_world();

		// 注册实体组件
		register_components();
	}

	auto Main::on_initialized() noexcept -> void
	{
		std::println("MainScene::on_initialized");
	}

	auto Main::on_unloaded() noexcept -> void
	{
		std::println("MainScene::on_unloaded");

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
		// todo: 更新
	}

	auto Main::render(sf::RenderWindow& window) noexcept -> void
	{
		// todo: 渲染
		static auto shape = [] noexcept -> sf::CircleShape
		{
			sf::CircleShape s{50};
			s.setFillColor(sf::Color::Red);

			return s;
		}();

		window.draw(shape);
	}
}
