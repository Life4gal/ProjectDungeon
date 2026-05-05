// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/projectile.hpp>

#include <utility/physics.hpp>

#include <component/projectile.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/sprite_animation.hpp>
#include <factory/detail/physics_body.hpp>
#include <factory/detail/physics_shape.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::factory
{
	using namespace component;

	auto Projectile::spawn(entt::registry& registry, const blueprint::Projectile& projectile, const entt::entity owner, sf::Vector2f direction) noexcept -> entt::entity
	{
		direction = direction.normalized();

		const auto [owner_position] = registry.get<const transform::Position>(owner);
		const auto transform = [&] -> blueprint::Transform
		{
			auto t = projectile.transform;
			t.x += owner_position.x;
			t.y += owner_position.y;

			return t;
		}();

		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, transform);
		// sprite_animation
		detail::attach(registry, entity, projectile.animation);
		// physics_body & physics_shape & velocity
		{
			const auto body_id = detail::create_attach(registry, entity, transform, projectile.physics_body);

			const auto shape_id = detail::create(body_id, transform, projectile.physics_shape);
			registry.emplace<projectile::PhysicsShape>(entity, shape_id);

			// TODO: 要解决飞弹刚发射就碰撞到自己有两种简易解决方案(不考虑在碰撞时判断)
			//  1.控制飞弹的初始位置
			//  2.控制飞弹的碰撞掩码
			//  
			// 第二种方式最简单,但是如果后续我们想开发一种可以通过发射者碰撞而改变轨迹的飞弹时要怎么办?如果允许飞弹命中友军怎么办?
			{
				const auto filter = [&] noexcept -> b2Filter
				{
					auto f = b2Shape_GetFilter(shape_id);

					if (const auto player = registry.all_of<tags::Player>(owner);
						player)
					{
						f.maskBits &= ~static_cast<std::uint64_t>(std::to_underlying(blueprint::PhysicsShapeType::PLAYER));
					}
					else
					{
						f.maskBits &= ~static_cast<std::uint64_t>(std::to_underlying(blueprint::PhysicsShapeType::ENEMY));
					}

					return f;
				}();
				b2Shape_SetFilter(shape_id, filter);
			}

			const auto pixels_velocity = direction * projectile.speed;
			const auto physics_velocity = utility::Physics::to_physics(pixels_velocity);
			b2Body_SetLinearVelocity(body_id, physics_velocity);
		}
		// type
		registry.emplace<projectile::Type>(entity, static_cast<projectile::Type>(projectile.type));
		// owner
		registry.emplace<projectile::Owner>(entity, owner);
		// damage
		registry.emplace<projectile::Damage>(entity, projectile.damage);
		// lifetime
		registry.emplace<projectile::Lifetime>(entity, sf::seconds(projectile.lifetime));
		// speed
		registry.emplace<projectile::Speed>(entity, projectile.speed);

		registry.emplace<tags::Projectile>(entity);

		return entity;
	}

	auto Projectile::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Projectile>();
		registry.destroy(view.begin(), view.end());
	}
}
