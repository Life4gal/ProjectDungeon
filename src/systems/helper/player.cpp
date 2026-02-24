// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/player.hpp>

#include <components/entity/player.hpp>

#include <components/animation.hpp>
#include <components/actor.hpp>
#include <components/transform.hpp>
#include <components/render.hpp>
#include <components/physics_body.hpp>

#include <systems/helper/physics_world.hpp>
#include <systems/helper/animation.hpp>

#include <game/constants.hpp>
#include <game/entity_type.hpp>
#include <game/user_data_entity.hpp>

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

		const auto entity = registry.create();

		// todo: 读取数据

		// animation
		{
			using namespace animation;

			const auto animation = Animation::get(registry, "AntleredRascal");
			const auto& begin_frame = animation->frames[0];

			registry.emplace<Timer>(entity, Timer{.duration = begin_frame.duration, .elapsed = sf::Time::Zero});
			registry.emplace<Index>(entity, Index{.total = animation->frames.size(), .current = 0});
			registry.emplace<animation::Animation>(entity, animation);

			if (animation->looping)
			{
				registry.emplace<Looping>(entity);
			}
		}
		// actor
		{
			registry.emplace<actor::Health>(entity, actor::Health{.current = 100, .max = 100});
			registry.emplace<actor::Mana>(entity, actor::Mana{.current = 20, .max = 20});
			registry.emplace<actor::Speed>(entity, actor::Speed{.speed = 200, .speed_squared = 200 * 200});

			// registry.emplace<actor::Velocity>(entity, sf::Vector2f{0, 0});
			// registry.emplace<actor::Direction>(entity, sf::Vector2f{0, 0});
		}
		// transform
		{
			using namespace transform;

			registry.emplace<Position>(entity, position);
			registry.emplace<Scale>(entity, scale);
			registry.emplace<Rotation>(entity, rotation);
		}
		// render
		{
			using namespace render;

			const auto& index = registry.get<const animation::Index>(entity);
			const auto& [animation] = registry.get<const animation::Animation>(entity);

			const auto& frame = animation->frames[index.current];

			// 纹理
			registry.emplace<Texture>(
				entity,
				Texture
				{
						.texture_path = frame.texture_path,
						.frame_x = frame.frame_x,
						.frame_y = frame.frame_y,
						.frame_width = frame.frame_width,
						.frame_height = frame.frame_height
				}
			);
			// 颜色
			registry.emplace<Color>(entity, sf::Color::White);
		}
		// physics
		{
			using namespace physics_body;

			const auto world_id = PhysicsWorld::id(registry);

			// 创建动态刚体
			b2BodyDef body_def = b2DefaultBodyDef();
			body_def.type = b2_dynamicBody;
			body_def.position = Constant::to_physics(position);
			body_def.rotation = b2MakeRot(rotation.asRadians());
			body_def.linearDamping = 10.0f;
			body_def.fixedRotation = true;
			body_def.userData = entity_to_user_data(entity);

			const auto body_id = b2CreateBody(world_id, &body_def);

			// 创建矩形碰撞体
			// todo: 如果动画的大小不一致,需要每帧更新碰撞体的大小吗? 还是说只根据第一帧的大小创建碰撞体就行了?
			const auto& texture = registry.get<const render::Texture>(entity);
			const auto half_width = Constant::to_physics(static_cast<float>(texture.frame_width) * scale.x * 0.5f);
			const auto half_height = Constant::to_physics(static_cast<float>(texture.frame_height) * scale.y * 0.5f);

			auto shape_def = b2DefaultShapeDef();
			// 设置碰撞过滤
			shape_def.filter.categoryBits = static_cast<std::uint64_t>(EntityType::PLAYER_GROUND);
			shape_def.filter.maskBits = CollisionMask::player_ground;
			shape_def.density = 1.0f;
			shape_def.material.friction = 0.0f;

			const auto box = b2MakeBox(half_width, half_height);

			b2CreatePolygonShape(body_id, &shape_def, &box);

			registry.emplace<BodyId>(entity, body_id);
		}
		// player
		{
			registry.ctx().emplace<player::Entity>(entity);
			registry.emplace<player::Movement>(entity);
		}

		return entity;
	}

	auto Player::handle_event(entt::registry& registry, const sf::Event& event) noexcept -> void
	{
		using namespace components;

		const auto [entity] = registry.ctx().get<const player::Entity>();
		auto& [movement_x, movement_y] = registry.get<player::Movement>(entity);

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

		const auto [entity] = registry.ctx().get<const player::Entity>();
		const auto [movement_x, movement_y] = registry.get<const player::Movement>(entity);

		b2Vec2 direction{0, 0};
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

		if (direction == b2Vec2_zero)
		{
			return;
		}

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		const auto speed = registry.get<const actor::Speed>(entity);
		const auto direction_normalized = b2Normalize(direction);
		const auto mass = b2Body_GetMass(body_id);

		const auto force = mass * direction_normalized * speed.speed;

		b2Body_ApplyForceToCenter(body_id, force, true);
	}
}
