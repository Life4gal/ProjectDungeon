// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/player.hpp>

#include <components/tags.hpp>
#include <components/name.hpp>
#include <components/player.hpp>

#include <config/collision_mask.hpp>
// 快速测试用
#include <config/level.hpp>
#include <components/dungeon.hpp>
#include <components/level.hpp>

#include <systems/helper/physics_world.hpp>
#include <systems/helper/transform.hpp>
#include <systems/helper/render.hpp>
#include <systems/helper/animation.hpp>
#include <systems/helper/physics_body.hpp>
#include <systems/helper/actor.hpp>

#include <game/log_entity.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <SFML/Window/Event.hpp>
#include <box2d/box2d.h>

namespace pd::systems::helper
{
	auto Player::spawn(
		entt::registry& registry,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> entt::entity
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(not registry.ctx().contains<player::Player>());

		// todo: 读取数据

		// 快速测试用
		const auto [level_entity] = registry.ctx().get<const dungeon::Level>();
		const auto& level = registry.get<const level::Level>(level_entity).level.get();
		const auto& animation_set = level.animation_set;

		constexpr auto animation_id = std::string_view{"AntleredRascal"};
		const auto animation_it = animation_set.find(animation_id);
		if (animation_it == animation_set.end())
		{
			// todo
			return entt::null;
		}
		const auto& animation = animation_it->second;

		const auto entity = registry.create();

		const auto texture_width = animation.texture_width;
		const auto texture_height = animation.texture_height;
		const auto size = sf::Vector2f{static_cast<float>(texture_width) * scale.x, static_cast<float>(texture_height) * scale.y};

		auto* physics_user_data = PhysicsWorld::to_user_data(entity);
		const auto physics_position = PhysicsWorld::physics_position_of(position);
		const auto physics_size = PhysicsWorld::physics_size_of(size);
		const auto physics_rotation = PhysicsWorld::physics_rotation_of(rotation);

		// tags
		registry.emplace<tags::player>(entity);
		// name
		registry.emplace<name::Name>(entity, std::format("玩家{}", entt::to_integral(entity)));
		// transform
		Transform::attach(registry, entity, position, scale, rotation);
		// render
		Render::attach(registry, entity, animation, config::RenderLayer::PLAYER_GROUND);
		// animation
		Animation::attach(registry, entity, animation);
		// physics_body
		{
			const auto world_id = PhysicsWorld::id(registry);

			// 创建动态刚体
			b2BodyDef body_def = b2DefaultBodyDef();
			body_def.type = b2_dynamicBody;
			body_def.position = physics_position;
			body_def.rotation = physics_rotation;
			body_def.linearDamping = 10.0f;
			body_def.fixedRotation = true;
			body_def.userData = physics_user_data;

			auto shape_def = b2DefaultShapeDef();
			// 设置碰撞过滤
			shape_def.filter.categoryBits = static_cast<std::uint64_t>(config::RenderLayer::PLAYER_GROUND);
			shape_def.filter.maskBits = config::CollisionMask::player_ground;
			shape_def.density = 1.0f;
			shape_def.material.friction = 0.0f;
			shape_def.enableContactEvents = true;
			shape_def.enableSensorEvents = true;

			// 创建矩形碰撞体
			const auto box = b2MakeBox(physics_size.x / 2, physics_size.y / 2);

			PhysicsBody::attach(registry, entity, world_id, body_def);
			PhysicsBody::attach_shape(registry, entity, shape_def, box);
		}
		// actor
		Actor::attach(registry, entity, 100, 20, 200);
		// player
		registry.emplace<player::Movement>(entity);

		log::on_create("玩家", entity, position, size, rotation, physics_position, physics_size);

		// 注册到上下文
		registry.ctx().emplace<player::Player>(entity);

		return entity;
	}

	auto Player::kill(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		const auto player_entity = get(registry);

		// physics_body
		PhysicsBody::deattach(registry, player_entity);

		log::on_destroy("玩家", player_entity);

		registry.destroy(player_entity);

		// 从上下文中移除
		registry.ctx().erase<player::Player>();
	}

	auto Player::handle_event(entt::registry& registry, const sf::Event& event) noexcept -> void
	{
		using namespace components;

		const auto player_entity = get(registry);
		auto& [movement_x, movement_y] = registry.get<player::Movement>(player_entity);

		if (const auto* kp = event.getIf<sf::Event::KeyPressed>())
		{
			using enum player::Movement::Type;
			using sf::Keyboard::Key;

			if (kp->code == Key::A)
			{
				movement_x = BACKWARD;
			}
			if (kp->code == Key::D)
			{
				movement_x = FORWARD;
			}
			if (kp->code == Key::W)
			{
				movement_y = BACKWARD;
			}
			if (kp->code == Key::S)
			{
				movement_y = FORWARD;
			}

			return;
		}

		if (const auto* kr = event.getIf<sf::Event::KeyReleased>())
		{
			using enum player::Movement::Type;
			using sf::Keyboard::Key;

			if (kr->code == Key::A)
			{
				movement_x = STAY;
			}
			if (kr->code == Key::D)
			{
				movement_x = STAY;
			}
			if (kr->code == Key::W)
			{
				movement_y = STAY;
			}
			if (kr->code == Key::S)
			{
				movement_y = STAY;
			}
		}
	}

	auto Player::apply_movement(entt::registry& registry) noexcept -> void
	{
		using namespace components;
		using enum player::Movement::Type;

		const auto player_entity = get(registry);
		const auto [movement_x, movement_y] = registry.get<const player::Movement>(player_entity);

		sf::Vector2f direction{0, 0};
		if (movement_x == FORWARD)
		{
			direction.x = 1;
		}
		else if (movement_x == BACKWARD)
		{
			direction.x = -1;
		}

		if (movement_y == FORWARD)
		{
			direction.y = 1;
		}
		else if (movement_y == BACKWARD)
		{
			direction.y = -1;
		}

		if (direction == sf::Vector2f{0, 0})
		{
			return;
		}

		const auto speed = Actor::get_speed(registry, player_entity);
		const auto direction_normalized = direction.normalized();
		const auto factor = direction_normalized * speed;

		PhysicsBody::apply_force_to_center(registry, player_entity, factor);
	}

	auto Player::get(entt::registry& registry) noexcept -> entt::entity
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<player::Player>());

		return registry.ctx().get<const player::Player>().entity;
	}
}
