// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main.hpp>

#include <print>

// ======================================
// CONFIG
// ======================================

#include <config/dungeon.hpp>

// ======================================
// SYSTEMS::HELPER
// ======================================

#include <systems/helper/physics_world.hpp>

#include <systems/helper/level.hpp>

#include <systems/helper/player.hpp>

// ======================================
// SYSTEMS::INITIALIZE
// ======================================

#include <systems/initialize/blueprint.hpp>
#include <systems/initialize/asset.hpp>
#include <systems/initialize/world.hpp>
#include <systems/initialize/physics_world.hpp>

#include <systems/initialize/debug.hpp>

// ======================================
// SYSTEMS::UPDATE
// ======================================

#include <systems/update/world.hpp>
#include <systems/update/check_room.hpp>
#include <systems/update/apply_player_movement.hpp>
#include <systems/update/physics_world.hpp>
#include <systems/update/sync_transform.hpp>
#include <systems/update/process_contact_and_sensor_events.hpp>
#include <systems/update/animation.hpp>

#include <systems/update/debug.hpp>

// ======================================
// SYSTEMS::RENDER
// ======================================

#include <systems/render/entity.hpp>

#include <systems/render/debug.hpp>

// ======================================
// DEPENDENCIES
// ======================================

#include <imgui.h>

namespace pd::scene
{
	auto Main::start_game() noexcept -> void
	{
		using namespace systems;

		auto& registry = scene_registry_;

		// 加载瓦片集
		const auto tileset = config::load_tileset("assets/configs/tileset.json");
		if (tileset.empty())
		{
			std::println("[错误] 无法加载瓦片集");
			return;
		}

		// 加载关卡
		const auto level = config::load_level("assets/configs/levels/level_01.json");
		if (level.id.empty())
		{
			std::println("[错误] 无法加载关卡");
			return;
		}

		if (not helper::Level::load_level(registry, level, tileset))
		{
			std::println("[错误] 加载关卡失败");
			return;
		}

		std::println("游戏开始! 当前房间: {}-{}", level.id, helper::Level::get_room_id(registry));
	}

	Main::Main(const std::reference_wrapper<entt::registry> global_registry) noexcept
		: Scene{global_registry} {}

	auto Main::on_loaded() noexcept -> void
	{
		std::println("MainScene::on_loaded");

		auto& registry = scene_registry_;

		using namespace systems;

		// ========================
		// 1.载入所有蓝图
		initialize::blueprint(registry);

		// ========================
		// 2.载入蓝图所需的资源
		initialize::asset(registry);

		// ========================
		// 3.初始化世界
		initialize::world(registry);

		// ========================
		// 3.初始化物理世界
		initialize::physics_world(registry);

		// ========================
		// n.测试用
		initialize::debug(registry);
	}

	auto Main::on_initialized() noexcept -> void
	{
		std::println("MainScene::on_initialized");

		// 开始游戏
		start_game();
	}

	auto Main::on_unloaded() noexcept -> void
	{
		std::println("MainScene::on_unloaded");

		auto& registry = scene_registry_;

		// =========
		// Dungeon
		{
			using namespace systems::helper;

			Level::unload_level(registry);
		}

		// =========
		// PhysicsWorld
		{
			using namespace systems::helper;

			PhysicsWorld::destroy(registry);
		}

		// todo: 更新全局统计数据等信息?
	}

	auto Main::handle_event(const sf::Event& event) noexcept -> void
	{
		using namespace systems;

		const auto& io = ImGui::GetIO();
		const auto want_capture_keyboard = io.WantCaptureKeyboard;
		const auto want_capture_mouse = io.WantCaptureMouse;

		if (want_capture_keyboard or want_capture_mouse)
		{
			return;
		}

		auto& registry = scene_registry_;

		helper::Player::handle_event(registry, event);
	}

	auto Main::update(const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		auto& registry = scene_registry_;

		// 更新世界
		update::world(registry, delta);
		// 检测房间是否清空
		update::check_room(registry);
		// 应用玩家移动
		update::apply_player_movement(registry, delta);
		// 更新物理世界
		update::physics_world(registry, delta);
		// 同步物理世界实体变换(transform)信息
		update::sync_transform(registry, delta);
		// 处理接触事件
		update::process_contact_and_sensor_events(registry, delta);
		// 更新动画
		update::animation(registry, delta);
	}

	auto Main::render(sf::RenderWindow& window) noexcept -> void
	{
		using namespace systems;

		auto& registry = scene_registry_;

		// 渲染实体
		render::entity(registry, window);
	}
}
