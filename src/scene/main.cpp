// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/main.hpp>

#include <print>

// ======================================
// ASSET
// ======================================

#include <asset/font.hpp>
#include <asset/texture.hpp>
#include <asset/sound.hpp>
#include <asset/music.hpp>

// ======================================
// BLUEPRINT
// ======================================

#include <blueprint/animation.hpp>

// ======================================
// COMPONENTS
// ======================================

#include <components/world.hpp>
#include <components/physics_world.hpp>

// ======================================
// SYSTEMS::INITIALIZE
// ======================================

#include <systems/initialize/blueprint.hpp>
#include <systems/initialize/asset.hpp>

#include <systems/initialize/debug.hpp>

// ======================================
// SYSTEMS::UPDATE
// ======================================

#include <systems/update/world.hpp>
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

#include <box2d/box2d.h>
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

		// ==========================
		// 将所有涉及registry.ctx的组件全部在这里创建
		// 其他*所有*地方都不应该创建,且应该通过ctx中的辅助类来访问
		// ==========================

		auto& registry = scene_registry_;

		// =========
		// World
		{
			using namespace components::world;

			// 避免可能的除0,初始delta设置为非0值
			registry.ctx().emplace<FrameDelta>(sf::seconds(1));
			registry.ctx().emplace<TotalElapsed>(sf::Time::Zero);
			registry.ctx().emplace<PlayElapsed>(sf::Time::Zero);
		}
		// =========
		// PhysicsWorld
		{
			using namespace components::physics_world;

			auto def = b2DefaultWorldDef();
			// 无重力世界
			def.gravity = b2Vec2_zero;
			const auto id = b2CreateWorld(&def);

			registry.ctx().emplace<Id>(id);
		}
		// =========
		// Asset
		{
			registry.ctx().emplace<asset::FontManager>();
			registry.ctx().emplace<asset::TextureManager>();
			registry.ctx().emplace<asset::SoundManager>();
			registry.ctx().emplace<asset::MusicManager>();
		}
		// =========
		// Blueprint
		{
			registry.ctx().emplace<blueprint::AnimationSet>();
		}

		using namespace systems;

		// ========================
		// 1.载入所有蓝图
		initialize::blueprint(registry);
		// ========================
		// 2.载入蓝图所需的资源
		initialize::asset(registry);

		// ========================
		// n.测试用
		initialize::debug(registry);
	}

	auto Main::on_initialized() noexcept -> void
	{
		std::println("MainScene::on_initialized");

		auto& registry = scene_registry_;
		//
	}

	auto Main::on_unloaded() noexcept -> void
	{
		std::println("MainScene::on_unloaded");

		// ==========================
		// 将所有涉及registry.ctx的组件全部在这里销毁(如果需要)
		// ==========================

		auto& registry = scene_registry_;

		// =========
		// PhysicsWorld
		{
			using namespace components::physics_world;

			const auto id = scene_registry_.ctx().get<Id>().id;
			b2DestroyWorld(id);
		}

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

		auto& registry = scene_registry_;
		// todo: 处理事件
	}

	auto Main::update(const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		auto& registry = scene_registry_;

		// 更新世界
		update::world(registry, delta);
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
