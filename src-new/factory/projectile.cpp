// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/projectile.hpp>

#include <utility/physics.hpp>

#include <component/projectile.hpp>
#include <component/name.hpp>

#include <assembly/transform.hpp>
#include <assembly/limited_life.hpp>
#include <assembly/render.hpp>
#include <assembly/collision.hpp>
#include <assembly/particle_emitter.hpp>

#include <prometheus/functional/functor.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <box2d/box2d.h>

namespace pd::factory
{
	using namespace component;

	auto Projectile::spawn(entt::registry& registry, const blueprint::Projectile& projectile, const entt::entity owner, sf::Vector2f direction) noexcept -> entt::entity
	{
		direction = direction.normalized();

		const auto [owner_position] = registry.get<const transform::Position>(owner);
		// TODO: 基于发射的方向给初始位置一个偏移
		const auto position_offset = direction * 10.f;
		const auto position = blueprint::Position{.x = owner_position.x + position_offset.x, .y = owner_position.y + position_offset.y};

		const auto entity = registry.create();

		const auto* owner_name = registry.try_get<name::Name>(owner);
		SPDLOG_INFO(
			"{}(0x{:08X})发射飞弹(Entity: 0x{:08X}, Position: ({:.1f}:{:.1f}), Direction: ({:.3f}:{:.3f}))",
			owner_name ? owner_name->name : "实体",
			entt::to_integral(owner),
			entt::to_integral(entity),
			position.x,
			position.y,
			direction.x,
			direction.y
		);

		// transform
		assembly::Transform::make(registry, entity, position);
		// limited_life
		assembly::LimitedLife::make(registry, entity, projectile.lifetime);
		// render
		assembly::Render::make(registry, entity, projectile.sprite, blueprint::RenderLayer::PROJECTILE);
		// collision
		assembly::Collision::make(registry, entity, projectile.collision, position);
		// owner
		registry.emplace<projectile::Owner>(entity, owner);
		// speed & velocity
		{
			const auto [body_id] = registry.get<collision::BodyId>(entity);
			const auto& [shape_ids] = registry.get<collision::ShapeIds>(entity);

			// TODO: 要解决飞弹刚发射就碰撞到自己有两种简易解决方案(不考虑在碰撞时判断)
			//  1.控制飞弹的初始位置
			//  2.控制飞弹的碰撞掩码
			//
			// 第二种方式最简单,但是如果后续我们想开发一种可以通过发射者碰撞而改变轨迹的飞弹时要怎么办?如果允许飞弹命中友军怎么办?
			//
			// TODO: 也许b2Filter::groupIndex能解决这个问题?待研究!
			for (const auto shape_id: shape_ids)
			{
				const auto filter = [&] noexcept -> b2Filter
				{
					auto f = b2Shape_GetFilter(shape_id);

					if (const auto player = registry.all_of<tags::Player>(owner);
						player)
					{
						f.maskBits &= ~std::to_underlying(blueprint::CollisionCategory::PLAYER);
					}
					else
					{
						f.maskBits &= ~std::to_underlying(blueprint::CollisionCategory::ENEMY);
					}

					return f;
				}();
				b2Shape_SetFilter(shape_id, filter);
			}

			const auto visitor = prometheus::functional::overloaded
			{
					[&](const blueprint::Trajectory::Straight& straight) noexcept -> void
					{
						// velocity
						const auto pixels_velocity = direction * straight.speed;
						const auto physics_velocity = utility::Physics::to_physics(pixels_velocity);
						b2Body_SetLinearVelocity(body_id, physics_velocity);

						// speed
						registry.emplace<projectile::Speed>(entity, straight.speed);
					},
					[&]([[maybe_unused]] const auto& unhandled) noexcept -> void
					{
						PROMETHEUS_PLATFORM_UNREACHABLE();
					}
			};

			std::visit(visitor, projectile.trajectory.trajectory);
		}
		// damage
		registry.emplace<projectile::Damage>(entity, projectile.damage);
		// 拖尾效果
		assembly::ParticleEmitter::make(registry, entity, projectile.particle_emitter);
		// tags
		registry.emplace<tags::Projectile>(entity);

		// 新生成
		registry.emplace<state::EntityNew>(entity);

		return entity;
	}

	auto Projectile::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Projectile>();
		registry.destroy(view.begin(), view.end());
	}
}
