// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/dungeon.hpp>

#include <utility/physics.hpp>

#include <designer/level.hpp>
#include <designer/player.hpp>

#include <component/dungeon.hpp>
#include <component/scheduled_task_context.hpp>
#include <component/renderer.hpp>

#include <factory/level.hpp>
#include <factory/player.hpp>

#include <helper/player_controller.hpp>
#include <helper/room.hpp>

#include <undertaker/scheduled_task.hpp>
#include <undertaker/collision.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::factory
{
	using namespace component;

	namespace
	{
		// 创建物理世界
		auto create_physics_world() noexcept -> void
		{
			auto& world_id = utility::Physics::world_id;

			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NULL(world_id), "重复创建物理世界");

			auto def = b2DefaultWorldDef();
			// 无重力世界(俯视角)
			def.gravity = b2Vec2_zero;
			world_id = b2CreateWorld(&def);
		}

		// 销毁物理世界
		auto destroy_physics_world() noexcept -> void
		{
			auto& world_id = utility::Physics::world_id;

			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(world_id), "物理世界未创建");

			b2DestroyWorld(world_id);
			world_id = b2_nullWorldId;
		}
	}

	auto Dungeon::create(entt::registry& registry, const blueprint::Dungeon& dungeon) noexcept -> void
	{
		// dungeon -> 所有关卡入口
		registry.ctx().emplace<dungeon::Entries>(dungeon.entries);
		// scheduled_task -> 上下文
		registry.ctx().emplace<scheduled_task::Context>();
		// renderer -> 渲染单元集+渲染命令队列
		registry.ctx().emplace<renderer::RenderItemSet>();
		registry.ctx().emplace<renderer::RenderCommandQueue>();

		// undertaker
		undertaker::ScheduledTask::watch(registry);
		undertaker::Collision::watch(registry);

		// 创建物理世界
		create_physics_world();

		// 创建关卡实体
		// TODO: 这部分内容应该移动到合适的位置
		const auto& level_config = dungeon.entries.front();
		manager::Random::seed(level_config.seed);
		const auto& level_blueprint = designer::Level::generate(level_config.horizontal_count, level_config.vertical_count, level_config.count, level_config.start_x, level_config.start_y);
		Level::create(registry, level_blueprint);

		// 创建玩家实体
		const auto player = designer::Player::test_character();
		const auto player_entity = Player::spawn(registry, player);
		helper::PlayerController::set_target(registry, player_entity);

		// 进入起始房间
		helper::Room::enter(registry, level_blueprint.start_position.x, level_blueprint.start_position.y);
	}

	auto Dungeon::destroy(entt::registry& registry) noexcept -> void
	{
		// 销毁玩家相关实体
		Player::destroy_all(registry);

		// 销毁关卡相关实体
		Level::destroy(registry);

		// 销毁物理世界
		destroy_physics_world();

		// undertaker
		//

		// dungeon -> 所有关卡入口
		registry.ctx().erase<dungeon::Entries>();
		// scheduled_task -> 上下文
		registry.ctx().erase<scheduled_task::Context>();
		// renderer -> 渲染单元集+渲染命令队列
		registry.ctx().erase<renderer::RenderItemSet>();
		registry.ctx().erase<renderer::RenderCommandQueue>();
	}
}
