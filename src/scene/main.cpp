// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main.hpp>

#include <print>

// ================
// SYSTEMS::INITIALIZE
// ================

#include <systems/initialize/world.hpp>
#include <systems/initialize/blueprint.hpp>
#include <systems/initialize/asset.hpp>

#include <systems/initialize/test.hpp>

// ================
// SYSTEMS::UPDATE
// ================

#include <systems/update/world.hpp>
#include <systems/update/animation.hpp>

// ================
// SYSTEMS::RENDER
// ================

#include <systems/render/animation.hpp>

// ================
// DEPENDENCIES
// ================

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <imgui.h>

namespace pd::scene
{
	auto Main::start_game() noexcept -> void
	{
		// todo
	}

	Main::Main(const std::reference_wrapper<entt::registry> global_registry) noexcept
		: Scene{global_registry},
		  physics_world_id_{b2_nullWorldId},
		  pause_{false} {}

	Main::~Main() noexcept
	{
		b2DestroyWorld(physics_world_id_);
	}

	auto Main::on_loaded() noexcept -> void
	{
		std::println("MainScene::on_loaded");

		// 创建物理世界
		{
			auto def = b2DefaultWorldDef();
			// 无重力世界
			def.gravity = b2Vec2_zero;

			physics_world_id_ = b2CreateWorld(&def);
		}

		// 注册实体组件
		{
			using namespace systems;

			// 载入蓝图数据
			initialize::blueprint(scene_registry_);
			// 载入资源
			// 我们在载入蓝图时创建各种映射(例如ctx::Map<asset::FontLoader>::emplace(registry))
			// 如此,在载入完蓝图后我们就可以基于映射去载入需要的资源
			initialize::asset(scene_registry_);

			// 初始化世界
			initialize::world(scene_registry_);

			// 测试
			initialize::test(scene_registry_);
		}
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
		using namespace systems;

		update::world(scene_registry_, delta);

		if (not pause_)
		{
			update::world_real(scene_registry_, delta);

			update::animation(scene_registry_, delta);
		}
	}

	auto Main::render(sf::RenderWindow& window) noexcept -> void
	{
		using namespace systems;

		render::animation(scene_registry_, window);
	}
}
