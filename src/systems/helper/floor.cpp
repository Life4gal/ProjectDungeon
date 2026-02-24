// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/floor.hpp>

#include <config/dungeon.hpp>

#include <components/entity/floor.hpp>

#include <components/transform.hpp>
#include <components/render.hpp>
#include <components/physics_body.hpp>

#include <systems/helper/physics_world.hpp>

#include <game/constants.hpp>
#include <game/entity_type.hpp>
#include <game/user_data_entity.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::helper
{
	namespace
	{
		template<bool Hole>
		auto do_spawn(
			entt::registry& registry,
			const config::Tile& tile,
			const sf::Vector2f position,
			const sf::Vector2f scale,
			const sf::Angle rotation
		) noexcept -> entt::entity
		{
			using namespace components;

			const auto entity = registry.create();

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

				registry.emplace<Texture>(
					entity,
					Texture
					{
							.texture_path = tile.texture_path,
							.frame_x = tile.texture_x,
							.frame_y = tile.texture_y,
							.frame_width = tile.texture_width,
							.frame_height = tile.texture_height
					}
				);

				registry.emplace<Color>(entity, sf::Color::White);
			}
			// physics
			{
				if constexpr (Hole)
				{
					using namespace physics_body;

					const auto world_id = PhysicsWorld::id(registry);

					// 创建静态刚体
					auto body_def = b2DefaultBodyDef();
					body_def.type = b2_staticBody;
					body_def.position = Constant::to_physics(position);
					body_def.rotation = b2MakeRot(rotation.asRadians());
					body_def.userData = entity_to_user_data(entity);

					const auto body_id = b2CreateBody(world_id, &body_def);

					// 创建矩形碰撞体
					const auto half_width = Constant::to_physics(static_cast<float>(tile.texture_width) * scale.x * 0.5f);
					const auto half_height = Constant::to_physics(static_cast<float>(tile.texture_height) * scale.y * 0.5f);

					auto shape_def = b2DefaultShapeDef();
					// 设置碰撞过滤
					shape_def.filter.categoryBits = static_cast<std::uint64_t>(EntityType::FLOOR_HOLE);
					shape_def.filter.maskBits = CollisionMask::floor_hole;

					const auto box = b2MakeBox(half_width, half_height);

					b2CreatePolygonShape(body_id, &shape_def, &box);

					registry.emplace<BodyId>(entity, body_id);
				}
				else
				{
					// 地板没有物理碰撞体
					// 如果需要感应器效果应该使用地板装饰物
				}
			}

			return entity;
		}
	}

	auto Floor::spawn(
		entt::registry& registry,
		const config::Tile& tile,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> entt::entity
	{
		return do_spawn<false>(registry, tile, position, scale, rotation);
	}

	auto Floor::spawn_hole(
		entt::registry& registry,
		const config::Tile& tile,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> entt::entity
	{
		return do_spawn<true>(registry, tile, position, scale, rotation);
	}
}
