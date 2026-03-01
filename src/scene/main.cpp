// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main.hpp>

#include <print>

// ======================================
// SYSTEMS::HELPER
// ======================================

#include <systems/helper/world.hpp>
#include <systems/helper/physics_world.hpp>

#include <systems/helper/dungeon.hpp>
#include <systems/helper/player.hpp>

// ======================================
// SYSTEMS::UPDATE
// ======================================

#include <systems/update/world.hpp>
#include <systems/update/update_room.hpp>
#include <systems/update/apply_player_movement.hpp>
#include <systems/update/physics_world.hpp>
#include <systems/update/sync_transform.hpp>
#include <systems/update/process_contact_events.hpp>
#include <systems/update/animation.hpp>

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
	namespace
	{
		// 快速测试用
		entt::entity g_player_entity;
	}

	auto Main::start_game() noexcept -> bool
	{
		using namespace systems;

		auto& registry = scene_registry_;

		// 加载地下城
		if (not helper::Dungeon::load(registry, "assets/configs/dungeons/dungeon-1.json"))
		{
			std::println("[错误] 加载地下城失败");
			return false;
		}

		std::println("游戏开始!");

		// 创建一个测试用实体
		// 64 / 16 == 4
		g_player_entity = helper::Player::spawn(registry, sf::Vector2f{500, 500}, sf::Vector2f{4, 4});

		return g_player_entity != entt::null;
	}

	Main::Main(const std::reference_wrapper<entt::registry> global_registry) noexcept
		: Scene{global_registry} {}

	auto Main::on_loaded() noexcept -> void
	{
		using namespace systems;

		std::println("MainScene::on_loaded");

		auto& registry = scene_registry_;

		// 创建世界
		helper::World::create(registry);

		// 创建物理世界
		helper::PhysicsWorld::create(registry);
	}

	auto Main::on_initialized() noexcept -> void
	{
		std::println("MainScene::on_initialized");

		// 开始游戏
		start_game();
	}

	auto Main::on_unloaded() noexcept -> void
	{
		using namespace systems;

		std::println("MainScene::on_unloaded");

		auto& registry = scene_registry_;

		// 销毁测试用实体
		helper::Player::kill(registry, g_player_entity);

		// 销毁物理世界
		helper::PhysicsWorld::destroy(registry);

		// 销毁世界
		helper::World::destroy(registry);

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

		if (g_player_entity != entt::null)
		{
			helper::Player::handle_event(registry, g_player_entity, event);
		}
	}

	auto Main::update(const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		auto& registry = scene_registry_;

		// 更新世界
		update::world(registry, delta);
		// 检测房间是否清空
		update::update_room(registry);
		// 应用玩家移动
		if (g_player_entity != entt::null)
		{
			update::apply_player_movement(registry, g_player_entity, delta);
		}
		// 更新物理世界
		update::physics_world(registry, delta);
		// 同步物理世界实体变换(transform)信息
		update::sync_transform(registry, delta);
		// 处理接触事件
		update::process_contact_events(registry, delta);
		// 更新动画
		update::animation(registry, delta);
	}

	auto Main::render(sf::RenderWindow& window) noexcept -> void
	{
		using namespace systems;

		auto& registry = scene_registry_;

		// 渲染实体
		render::entity(registry, window);

		render::debug(registry, window);
	}
}
