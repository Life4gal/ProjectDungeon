// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/projectile.hpp>

#include <utility/physics.hpp>

#include <component/projectile.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/render.hpp>
#include <factory/detail/collision.hpp>

#include <prometheus/functional/functor.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
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

		// transform
		detail::attach(registry, entity, position);
		// render
		detail::attach(registry, entity, projectile.sprite, blueprint::RenderLayer::PROJECTILE);
		// collision
		detail::attach(registry, entity, projectile.collision, position);
		{
			// collision
		}
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
					[&](const blueprint::Trajectory::straight& straight) noexcept -> void
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
		// lifetime
		registry.emplace<projectile::Lifetime>(entity, sf::seconds(projectile.lifetime));
		// damage
		registry.emplace<projectile::Damage>(entity, projectile.damage);
		// tags
		registry.emplace<tags::Projectile>(entity);

		return entity;
	}

	auto Projectile::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Projectile>();
		registry.destroy(view.begin(), view.end());
	}
}
