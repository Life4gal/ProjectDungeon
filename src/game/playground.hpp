// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <algorithm>
#include <ranges>
#include <print>

#include <prometheus/functional/enumeration.hpp>
#include <prometheus/platform/os.hpp>

#include <box2d/box2d.h>

#include <SFML/Graphics.hpp>

#include <imgui.h>

#include <game/defines.hpp>
#include <game/constants.hpp>

namespace playground
{
	enum class EntityType : std::uint16_t
	{
		// [0~3]

		// 墙
		WALL = 0b0000'0000'0000'0001,
		// 地板
		FLOOR = 0b0000'0000'0000'0010,
		// 地形/装饰物
		DECORATION = 0b0000'0000'0000'0100,

		// [4~7]

		// 敌人实体
		ENEMY = 0b0000'0000'0001'0000,
		// 玩家实体
		PLAYER = 0b0000'0000'0010'0000,

		// [8~11]

		// 飞弹
		PROJECTILE = 0b0000'0001'0000'0000,

		PROMETHEUS_MAGIC_ENUM_FLAG
	};

	enum class RenderLayer : std::uint8_t
	{
		// 背景
		BACKGROUND,

		// 地板
		FLOOR,

		// 墙
		WALL,
		// 地形/装饰物
		DECORATION,

		// 敌人实体
		ENEMY,
		// 玩家实体
		PLAYER,

		// 飞弹
		PROJECTILE,
	};

	[[nodiscard]] constexpr auto operator<(const RenderLayer lhs, const RenderLayer rhs) noexcept -> bool
	{
		return static_cast<std::uint32_t>(lhs) < static_cast<std::uint32_t>(rhs);
	}

	class Entity
	{
	protected:
		// box2d --> 米
		// SFML --> 像素
		b2BodyId body_;

		bool active_;
		RenderLayer render_layer_;

	public:
		explicit Entity(const RenderLayer render_layer) noexcept
			: body_{b2_nullBodyId},
			  active_{true},
			  render_layer_{render_layer} {}

		Entity(const Entity&) noexcept = delete;
		auto operator=(const Entity&) noexcept -> Entity& = delete;

		Entity(Entity&&) noexcept = default;
		auto operator=(Entity&&) noexcept -> Entity& = default;

		virtual ~Entity() noexcept
		{
			// 销毁body
			b2DestroyBody(body_);
		}

		[[nodiscard]] virtual auto type() const noexcept -> EntityType = 0;

		virtual auto update(float delta) noexcept -> void = 0;

		virtual auto render(sf::RenderWindow& window) noexcept -> void = 0;

		// 这个接口本不应该,不过简化逻辑
		[[nodiscard]] auto body() const noexcept -> b2BodyId
		{
			return body_;
		}

		[[nodiscard]] auto is_active() const noexcept -> bool
		{
			return active_;
		}

		[[nodiscard]] auto render_layer() const noexcept -> RenderLayer
		{
			return render_layer_;
		}
	};

	class ActorEntity : public Entity
	{
	public:
		//

	private:
		[[nodiscard]] static auto create_physics(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size,
			const EntityType type
		) noexcept -> b2BodyId
		{
			const auto physics_position = pd::Constant::to_physics(position);
			const auto physics_size = pd::Constant::to_physics(size);

			b2BodyId body;
			// 创建BODY
			{
				auto def = b2DefaultBodyDef();
				def.type = b2_dynamicBody;
				def.position = physics_position;
				// todo: 需不需要可以旋转?
				def.fixedRotation = true;
				def.linearDamping = 5;

				body = b2CreateBody(world, &def);
			}
			// 创建SHAPE
			{
				auto def = b2DefaultShapeDef();
				def.density = 1;
				def.material.friction = 0.3f;
				def.material.restitution = 0.05f;
				def.filter.categoryBits = static_cast<uint64_t>(type);
				// 可以和墙壁/地板/装饰物/敌人/玩家/飞弹碰撞
				def.filter.maskBits = static_cast<uint64_t>(
					EntityType::WALL | EntityType::FLOOR | EntityType::DECORATION |
					EntityType::ENEMY | EntityType::PLAYER |
					EntityType::PROJECTILE
				);
				// todo
				// 踩地板触发传感器
				def.enableSensorEvents = true;
				// 与敌人/玩家/飞弹碰撞
				def.enableContactEvents = true;
				def.enableHitEvents = true;
				const auto box = b2MakeBox(physics_size.x / 2, physics_size.y / 2);

				b2CreatePolygonShape(body, &def, &box);
			}

			return body;
		}

		sf::RectangleShape shape_;
		// 最大移动速度(定值)
		float move_speed_max_;
		// 当前最大移动速度(可变,例如因踩中地板变化)
		float move_speed_current_max_;
		// 避免每次计算
		float move_speed_current_physics_max_;
		float move_speed_current_physics_max_squared_;

		// todo: 限制/控制加速度?
		// todo: 踩中不同地板可能造成[当前最大速度/当前最大加速度]变化,[当前最大移动速度]在普通地板时复原为[最大移动速度]

		auto limit_max_speed() const noexcept -> void
		{
			auto physics_velocity = b2Body_GetLinearVelocity(body_);

			if (const auto physics_speed_squared = b2LengthSquared(physics_velocity);
				physics_speed_squared > move_speed_current_physics_max_squared_)
			{
				const auto physics_speed = std::sqrt(physics_speed_squared);

				physics_velocity *= move_speed_current_physics_max_ / physics_speed;
				b2Body_SetLinearVelocity(body_, physics_velocity);
			}
		}

		auto sync_position_and_rotation() noexcept -> void
		{
			const auto physics_position = b2Body_GetPosition(body_);
			const auto physics_rotation = b2Body_GetRotation(body_);
			const auto physics_rotation_radians = b2Rot_GetAngle(physics_rotation);

			shape_.setPosition(pd::Constant::from_physics(physics_position));
			shape_.setRotation(sf::radians(physics_rotation_radians));
		}

	public:
		ActorEntity(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size,
			const float move_speed_max,
			const EntityType type
		) noexcept
			: Entity{type == EntityType::ENEMY ? RenderLayer::ENEMY : RenderLayer::PLAYER},
			  shape_{size},
			  move_speed_max_{move_speed_max},
			  move_speed_current_max_{move_speed_max},
			  move_speed_current_physics_max_{pd::Constant::to_physics(move_speed_max)},
			  move_speed_current_physics_max_squared_{move_speed_current_physics_max_ * move_speed_current_physics_max_}
		{
			// 创建body
			body_ = create_physics(world, position, size, type);

			// 设置UserData
			b2Body_SetUserData(body_, this);

			// 敌人/玩家颜色
			shape_.setFillColor(type == EntityType::ENEMY ? sf::Color::Red : sf::Color::Green);
			// 中心对齐
			shape_.setOrigin(shape_.getSize() / 2.f);
		}

		auto update(const float delta) noexcept -> void override
		{
			// 限制最大速度
			limit_max_speed();

			// 同步位置/旋转
			sync_position_and_rotation();
		}

		auto render(sf::RenderWindow& window) noexcept -> void override
		{
			if (not active_)
			{
				return;
			}

			window.draw(shape_);
		}

		[[nodiscard]] auto position() const noexcept -> sf::Vector2f
		{
			return shape_.getPosition();
		}

		[[nodiscard]] auto size() const noexcept -> sf::Vector2f
		{
			return shape_.getSize();
		}

		[[nodiscard]] auto velocity() const noexcept -> sf::Vector2f
		{
			const auto physics_velocity = b2Body_GetLinearVelocity(body_);

			return pd::Constant::from_physics(physics_velocity);
		}

		auto apply_force(const b2Vec2 direction) const noexcept -> void
		{
			if (direction == b2Vec2_zero)
			{
				return;
			}

			const auto direction_normalized = b2Normalize(direction);
			const auto force = b2Body_GetMass(body_) * direction_normalized * move_speed_current_max_;

			b2Body_ApplyForceToCenter(body_, force, true);
		}
	};

	// 敌人
	class EnemyEntity final : public ActorEntity
	{
	public:
		EnemyEntity(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size,
			const float move_speed_max
		) noexcept
			: ActorEntity{world, position, size, move_speed_max, EntityType::ENEMY} {}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::ENEMY;
		}
	};

	// 玩家
	class PlayerEntity final : public ActorEntity
	{
	public:
		PlayerEntity(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size,
			const float move_speed_max
		) noexcept
			: ActorEntity{world, position, size, move_speed_max, EntityType::PLAYER} {}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::PLAYER;
		}
	};

	// 墙壁
	class WallEntity final : public Entity
	{
	public:
		//

	private:
		[[nodiscard]] static auto create_physics(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size
		) noexcept -> b2BodyId
		{
			const auto physics_position = pd::Constant::to_physics(position);
			const auto physics_size = pd::Constant::to_physics(size);

			b2BodyId body;
			// 创建BODY
			{
				auto def = b2DefaultBodyDef();
				def.type = b2_staticBody;
				def.position = physics_position;
				def.fixedRotation = true;

				body = b2CreateBody(world, &def);
			}
			// 创建SHAPE
			{
				auto def = b2DefaultShapeDef();
				def.density = 0;
				def.material.friction = 0.5f;
				def.material.restitution = 0.7f;
				def.filter.categoryBits = static_cast<uint64_t>(EntityType::WALL);
				// 可以和敌人/玩家/飞弹碰撞
				def.filter.maskBits = static_cast<uint64_t>(
					EntityType::ENEMY | EntityType::PLAYER |
					EntityType::PROJECTILE
				);
				const auto box = b2MakeBox(physics_size.x / 2, physics_size.y / 2);

				b2CreatePolygonShape(body, &def, &box);
			}

			return body;
		}

		sf::RectangleShape shape_;

	public:
		WallEntity(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size
		) noexcept
			: Entity{RenderLayer::WALL},
			  shape_{size}
		{
			// 创建body
			body_ = create_physics(world, position, size);

			// 设置UserData
			b2Body_SetUserData(body_, this);

			// 墙颜色
			shape_.setFillColor(sf::Color::Black);
			// 中心对齐
			shape_.setOrigin(shape_.getSize() / 2.f);
			// 初始化位置(因为我们不会在update中更新位置,所以必须初始化)
			shape_.setPosition(position);
		}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::WALL;
		}

		auto update(float delta) noexcept -> void override
		{
			// 墙无需更新
			// 除非墙是动画
		}

		auto render(sf::RenderWindow& window) noexcept -> void override
		{
			window.draw(shape_);
		}
	};

	// 地板
	class FloorEntity final : public Entity
	{
	public:
		enum class Type : std::uint8_t
		{
			// 普通地板
			NORMAL,
			// 冰地板
			ICE,
			// 泥地板
			MUD,
		};

	private:
		[[nodiscard]] static auto create_physics(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size
		) noexcept -> b2BodyId
		{
			const auto physics_position = pd::Constant::to_physics(position);
			const auto physics_size = pd::Constant::to_physics(size);

			b2BodyId body;
			// 创建BODY
			{
				auto def = b2DefaultBodyDef();
				def.type = b2_staticBody;
				def.position = physics_position;
				def.fixedRotation = true;
				def.linearDamping = 5;

				body = b2CreateBody(world, &def);
			}
			// 创建SHAPE
			{
				auto def = b2DefaultShapeDef();
				// 传感器不阻止移动,可以检测接触
				def.isSensor = true;
				def.filter.categoryBits = static_cast<uint64_t>(EntityType::FLOOR);
				// 可以和敌人/玩家碰撞
				def.filter.maskBits = static_cast<uint64_t>(
					EntityType::ENEMY | EntityType::PLAYER
				);
				// 踩地板触发传感器
				def.enableSensorEvents = true;
				const auto box = b2MakeBox(physics_size.x / 2, physics_size.y / 2);

				b2CreatePolygonShape(body, &def, &box);
			}

			return body;
		}

		sf::RectangleShape shape_;

	public:
		FloorEntity(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size,
			const Type type
		) noexcept
			: Entity{RenderLayer::FLOOR},
			  shape_{size}
		{
			// 创建body
			body_ = create_physics(world, position, size);

			// 设置UserData
			b2Body_SetUserData(body_, this);

			// 地板颜色
			switch (type)
			{
				case Type::NORMAL:
				{
					// 绿色
					shape_.setFillColor(sf::Color::Green);
					// 深绿色边框
					shape_.setOutlineColor(sf::Color(0, 150, 0));
					shape_.setOutlineThickness(1.0f);
					break;
				}
				case Type::ICE:
				{
					// 淡蓝色
					shape_.setFillColor(sf::Color(200, 230, 255));
					// 蓝色边框
					shape_.setOutlineColor(sf::Color(150, 200, 255));
					shape_.setOutlineThickness(1.0f);
					break;
				}
				case Type::MUD:
				{
					// 泥棕色
					shape_.setFillColor(sf::Color(101, 67, 33));
					// 深棕色边框
					shape_.setOutlineColor(sf::Color(81, 47, 13));
					shape_.setOutlineThickness(1.0f);
					break;
				}
			}
			// 中心对齐
			shape_.setOrigin(shape_.getSize() / 2.f);
			// 初始化位置(因为我们不会在update中更新位置,所以必须初始化)
			shape_.setPosition(position);
		}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::FLOOR;
		}

		auto update(float delta) noexcept -> void override
		{
			// 地板无需更新
			// 除非地板是动画
		}

		auto render(sf::RenderWindow& window) noexcept -> void override
		{
			window.draw(shape_);
		}
	};

	// 地形/装饰物
	class DecorationEntity final : public Entity
	{
	public:
		//

	private:
		[[nodiscard]] static auto create_physics(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size
		) noexcept -> b2BodyId
		{
			const auto physics_position = pd::Constant::to_physics(position);
			const auto physics_size = pd::Constant::to_physics(size);

			b2BodyId body;
			// 创建BODY
			{
				auto def = b2DefaultBodyDef();
				def.type = b2_staticBody;
				def.position = physics_position;
				def.fixedRotation = true;

				body = b2CreateBody(world, &def);
			}
			// 创建SHAPE
			{
				auto def = b2DefaultShapeDef();
				def.density = 0;
				def.material.friction = 0.5f;
				def.material.restitution = 0.7f;
				def.filter.categoryBits = static_cast<uint64_t>(EntityType::DECORATION);
				// 可以和敌人/玩家/飞弹碰撞
				// todo: 如果是一个可以移动的装饰物呢?如果可以移动,则还可以和墙壁/装饰物碰撞
				def.filter.maskBits = static_cast<uint64_t>(
					EntityType::ENEMY | EntityType::PLAYER |
					EntityType::PROJECTILE
				);
				const auto box = b2MakeBox(physics_size.x / 2, physics_size.y / 2);

				b2CreatePolygonShape(body, &def, &box);
			}

			return body;
		}

		sf::RectangleShape shape_;

	public:
		DecorationEntity(
			const b2WorldId world,
			const sf::Vector2f position,
			const sf::Vector2f size
		) noexcept
			: Entity{RenderLayer::DECORATION},
			  shape_{size}
		{
			// 创建body
			body_ = create_physics(world, position, size);

			// 设置UserData
			b2Body_SetUserData(body_, this);

			// 地形/装饰物颜色
			shape_.setFillColor(sf::Color::Yellow);
			// 中心对齐
			shape_.setOrigin(shape_.getSize() / 2.f);
			// 初始化位置(因为我们不会在update中更新位置,所以必须初始化)
			shape_.setPosition(position);
		}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::DECORATION;
		}

		auto update(float delta) noexcept -> void override
		{
			// 地形/装饰物无需更新
			// 除非地形/装饰物是动画
		}

		auto render(sf::RenderWindow& window) noexcept -> void override
		{
			window.draw(shape_);
		}
	};

	// 飞弹
	// todo: ProjectileEntity的设计一定程度上和ActorEntity重叠
	class ProjectileEntity : public Entity
	{
	public:
		//

	private:
		// 飞行时长(生命周期)
		float lifetime_;
		float lifetime_current_;

	public:
		explicit ProjectileEntity(const float lifetime) noexcept
			: Entity{RenderLayer::PROJECTILE},
			  lifetime_{lifetime},
			  lifetime_current_{0} {}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::PROJECTILE;
		}

		auto update(const float delta) noexcept -> void override
		{
			lifetime_current_ += delta;
			if (lifetime_current_ >= lifetime_)
			{
				active_ = false;
			}
		}

		// 处理碰撞
		// 这个接口本不应该在这里实现,不过目前只有打印功能,实现也无所谓
		virtual auto on_collide(Entity& entity) noexcept -> void
		{
			// todo: 碰撞特效/音效?

			// todo: 在这里结算伤害?
			if (const auto type = entity.type();
				type == EntityType::WALL)
			{
				std::println("命中WALL");
			}
			else if (type == EntityType::DECORATION)
			{
				std::println("命中DECORATION");
			}
			else if (type == EntityType::ENEMY)
			{
				std::println("命中ENEMY");
			}
			else if (type == EntityType::PLAYER)
			{
				std::println("命中PLAYER");
			}
		}

		[[nodiscard]] virtual auto position() const noexcept -> sf::Vector2f = 0;

		[[nodiscard]] virtual auto size() const noexcept -> sf::Vector2f = 0;

		[[nodiscard]] virtual auto velocity() const noexcept -> sf::Vector2f = 0;
	};

	// 基础飞弹
	// 命中目标后消失
	class BaseProjectileEntity final : public ProjectileEntity
	{
	public:
		//

	private:
		[[nodiscard]] static auto create_physics(
			const b2WorldId world,
			const EntityType owner,
			const sf::Vector2f position,
			const float radius,
			const sf::Vector2f velocity
		) noexcept -> b2BodyId
		{
			const auto physics_position = pd::Constant::to_physics(position);
			const auto physics_radius = pd::Constant::to_physics(radius);
			const auto physics_velocity = pd::Constant::to_physics(velocity);

			b2BodyId body;
			// 创建BODY
			{
				auto def = b2DefaultBodyDef();
				def.type = b2_dynamicBody;
				def.position = physics_position;
				def.linearVelocity = physics_velocity;
				def.linearDamping = 0.05f;
				def.fixedRotation = false;
				def.isBullet = true;

				body = b2CreateBody(world, &def);
			}
			// 创建SHAPE
			{
				auto def = b2DefaultShapeDef();
				def.density = 0.5f;
				def.material.friction = 0;
				def.material.restitution = 0.8f;
				def.filter.categoryBits = static_cast<uint64_t>(EntityType::PROJECTILE);
				// 可以和墙壁/装饰物/敌人/玩家/碰撞
				def.filter.maskBits = static_cast<uint64_t>(
					EntityType::WALL | EntityType::DECORATION |
					(owner == EntityType::ENEMY ? EntityType::PLAYER : EntityType::ENEMY)
				);
				// 与敌人/玩家碰撞
				def.enableContactEvents = true;
				def.enableHitEvents = true;
				const auto circle = b2Circle{.center = b2Vec2_zero, .radius = physics_radius};

				b2CreateCircleShape(body, &def, &circle);
			}

			return body;
		}

		// 主体
		sf::CircleShape shape_;
		// 拖尾效果
		sf::ConvexShape tail_;

		auto sync_position_and_rotation() noexcept -> void
		{
			const auto physics_position = b2Body_GetPosition(body_);
			const auto physics_rotation = b2Body_GetRotation(body_);
			const auto physics_rotation_radians = b2Rot_GetAngle(physics_rotation);

			shape_.setPosition(pd::Constant::from_physics(physics_position));
			shape_.setRotation(sf::radians(physics_rotation_radians));
		}

	public:
		BaseProjectileEntity(
			const b2WorldId world,
			const EntityType owner,
			const sf::Vector2f position,
			const float radius,
			const sf::Vector2f velocity,
			const float lifetime
		) noexcept
			: ProjectileEntity{lifetime},
			  shape_{radius},
			  // 三角形拖尾
			  tail_{3}
		{
			// 创建body
			body_ = create_physics(world, owner, position, radius, velocity);

			// 设置UserData
			b2Body_SetUserData(body_, this);

			// 飞弹颜色
			shape_.setFillColor(sf::Color::Red);
			// 拖尾颜色
			tail_.setFillColor({255, 100, 100, 150});
			// 中心对齐
			shape_.setOrigin({shape_.getRadius(), shape_.getRadius()});
			// 不需要初始化位置(因为我们会在update中更新位置)
			// 不过其实初始化也无所谓?
			shape_.setPosition(position);
		}

		auto update(const float delta) noexcept -> void override
		{
			ProjectileEntity::update(delta);

			// 低速弹跳飞弹直接销毁
			// todo: 低速阈值
			constexpr auto min_velocity_2 = 5 * 5;
			if (const auto velocity = b2Body_GetLinearVelocity(body_);
				b2LengthSquared(velocity) < min_velocity_2)
			{
				active_ = false;
			}

			if (not active_)
			{
				return;
			}

			// 同步位置/旋转
			sync_position_and_rotation();
		}

		auto render(sf::RenderWindow& window) noexcept -> void override
		{
			if (not active_)
			{
				return;
			}

			const auto position = shape_.getPosition();
			const auto radius = shape_.getRadius();

			const auto physics_velocity = b2Body_GetLinearVelocity(body_);
			const auto physics_velocity_normalized = b2Normalize(physics_velocity);

			const auto direction = sf::Vector2f{physics_velocity_normalized.x, physics_velocity_normalized.y};
			const auto left_perpendicular = sf::Vector2f{-direction.y, direction.x};

			// 先渲染拖尾
			tail_.setPoint(0, position + direction * radius);
			tail_.setPoint(1, position - direction * (radius * 2) + left_perpendicular * radius);
			tail_.setPoint(2, position - direction * (radius * 2) - left_perpendicular * radius);
			window.draw(tail_);

			// 再渲染主体(可以遮住拖尾与主体重叠部分)
			window.draw(shape_);
		}

		auto on_collide(Entity& entity) noexcept -> void override
		{
			ProjectileEntity::on_collide(entity);

			// 命中后直接销毁飞弹
			active_ = false;
		}

		auto position() const noexcept -> sf::Vector2f override
		{
			return shape_.getPosition();
		}

		[[nodiscard]] auto size() const noexcept -> sf::Vector2f override
		{
			return {shape_.getRadius(), shape_.getRadius()};
		}

		[[nodiscard]] auto velocity() const noexcept -> sf::Vector2f override
		{
			const auto physics_velocity = b2Body_GetLinearVelocity(body_);

			return pd::Constant::from_physics(physics_velocity);
		}
	};

	// 弹跳飞弹
	// 命中目标后继续弹跳
	class BounceProjectileEntity final : public ProjectileEntity
	{
	public:
		using size_type = std::uint32_t;

	private:
		[[nodiscard]] static auto create_physics(
			const b2WorldId world,
			const EntityType owner,
			const sf::Vector2f position,
			const float radius,
			const sf::Vector2f velocity
		) noexcept -> b2BodyId
		{
			const auto physics_position = pd::Constant::to_physics(position);
			const auto physics_radius = pd::Constant::to_physics(radius);
			const auto physics_velocity = pd::Constant::to_physics(velocity);

			b2BodyId body;
			// 创建BODY
			{
				auto def = b2DefaultBodyDef();
				def.type = b2_dynamicBody;
				def.position = physics_position;
				def.linearVelocity = physics_velocity;
				def.linearDamping = 0.05f;
				def.fixedRotation = false;
				def.isBullet = true;

				body = b2CreateBody(world, &def);
			}
			// 创建SHAPE
			{
				auto def = b2DefaultShapeDef();
				def.density = 0.5f;
				def.material.friction = 0;
				def.material.restitution = 0.8f;
				def.filter.categoryBits = static_cast<uint64_t>(EntityType::PROJECTILE);
				// 可以和墙壁/装饰物/敌人/玩家/碰撞
				def.filter.maskBits = static_cast<uint64_t>(
					EntityType::WALL | EntityType::DECORATION |
					(owner == EntityType::ENEMY ? EntityType::PLAYER : EntityType::ENEMY)
				);
				// 与敌人/玩家碰撞
				def.enableContactEvents = true;
				def.enableHitEvents = true;
				const auto circle = b2Circle{.center = b2Vec2_zero, .radius = physics_radius};

				b2CreateCircleShape(body, &def, &circle);
			}

			return body;
		}

		// 弹跳次数(撞击墙壁/装饰物)
		size_type bounces_;
		size_type bounces_current_;

		// 主体
		sf::CircleShape shape_;
		// 拖尾效果
		sf::ConvexShape tail_;

		auto sync_position_and_rotation() noexcept -> void
		{
			const auto physics_position = b2Body_GetPosition(body_);
			const auto physics_rotation = b2Body_GetRotation(body_);
			const auto physics_rotation_radians = b2Rot_GetAngle(physics_rotation);

			shape_.setPosition(pd::Constant::from_physics(physics_position));
			shape_.setRotation(sf::radians(physics_rotation_radians));
		}

	public:
		BounceProjectileEntity(
			const b2WorldId world,
			const EntityType owner,
			const sf::Vector2f position,
			const float radius,
			const sf::Vector2f velocity,
			const float lifetime,
			const size_type bounces
		) noexcept
			: ProjectileEntity{lifetime},
			  bounces_{bounces},
			  bounces_current_{0},
			  shape_{radius},
			  // 三角形拖尾
			  tail_{3}
		{
			// 创建body
			body_ = create_physics(world, owner, position, radius, velocity);

			// 设置UserData
			b2Body_SetUserData(body_, this);

			// 飞弹颜色
			shape_.setFillColor(sf::Color::Red);
			// 拖尾颜色
			tail_.setFillColor({255, 100, 100, 150});
			// 中心对齐
			shape_.setOrigin({shape_.getRadius(), shape_.getRadius()});
			// 不需要初始化位置(因为我们会在update中更新位置)
			// 不过其实初始化也无所谓?
			shape_.setPosition(position);
		}

		auto update(const float delta) noexcept -> void override
		{
			ProjectileEntity::update(delta);

			// 低速弹跳飞弹直接销毁
			// todo: 低速阈值
			constexpr auto min_velocity_2 = 5 * 5;
			if (const auto velocity = b2Body_GetLinearVelocity(body_);
				b2LengthSquared(velocity) < min_velocity_2)
			{
				active_ = false;
			}

			if (not active_)
			{
				return;
			}

			// 同步位置/旋转
			sync_position_and_rotation();
		}

		auto render(sf::RenderWindow& window) noexcept -> void override
		{
			const auto position = shape_.getPosition();
			const auto radius = shape_.getRadius();

			const auto physics_velocity = b2Body_GetLinearVelocity(body_);
			const auto physics_velocity_normalized = b2Normalize(physics_velocity);

			const auto direction = sf::Vector2f{physics_velocity_normalized.x, physics_velocity_normalized.y};
			const auto left_perpendicular = sf::Vector2f{-direction.y, direction.x};

			// 先渲染拖尾
			tail_.setPoint(0, position + direction * radius);
			tail_.setPoint(1, position - direction * (radius * 2) + left_perpendicular * radius);
			tail_.setPoint(2, position - direction * (radius * 2) - left_perpendicular * radius);
			window.draw(tail_);

			// 再渲染主体(可以遮住拖尾与主体重叠部分)
			window.draw(shape_);
		}

		// 处理反弹
		auto on_collide(Entity& entity) noexcept -> void override
		{
			ProjectileEntity::on_collide(entity);

			bounces_current_ += 1;
			if (bounces_current_ >= bounces_)
			{
				active_ = false;
			}

			if (not active_)
			{
				return;
			}

			// 每次反弹都稍微降低弹性
			// todo: create_physics保存shape id?假定只有一个?
			{
				PROMETHEUS_PLATFORM_ASSUME(b2Body_GetShapeCount(body_) == 1);

				b2ShapeId shape_id;
				b2Body_GetShapes(body_, &shape_id, 1);

				// todo: 弹性减少系数
				constexpr auto restitution_factor = 0.9f;
				const auto restitution = b2Shape_GetRestitution(shape_id);

				b2Shape_SetRestitution(shape_id, restitution * restitution_factor);
			}

			// 每次反弹后稍微减少速度
			const auto physics_velocity = b2Body_GetLinearVelocity(body_);
			// todo: 减速系数
			// 减速系数可以随着反弹次数增加而增大
			constexpr auto slowdown_factor_default = 0.95f;
			auto slowdown_factor = slowdown_factor_default - (bounces_current_ * 0.05f);
			// 不低于0.3倍
			slowdown_factor = std::ranges::max(slowdown_factor, 0.3f);
			// 设置速度
			const auto new_physics_velocity = physics_velocity * slowdown_factor;
			b2Body_SetLinearVelocity(body_, new_physics_velocity);
		}

		auto position() const noexcept -> sf::Vector2f override
		{
			return shape_.getPosition();
		}

		[[nodiscard]] auto size() const noexcept -> sf::Vector2f override
		{
			return {shape_.getRadius(), shape_.getRadius()};
		}

		[[nodiscard]] auto velocity() const noexcept -> sf::Vector2f override
		{
			const auto physics_velocity = b2Body_GetLinearVelocity(body_);

			return pd::Constant::from_physics(physics_velocity);
		}
	};

	// todo: 其他类型飞弹
	// 可以直接穿过地形/装饰物的飞弹?
	// 范围性飞弹?

	class SensorProcessor final
	{
	public:
		static auto step(const b2WorldId world) noexcept -> void
		{
			const auto events = b2World_GetSensorEvents(world);

			// if (events.beginCount != 0)
			// {
			// 	std::println("b2SensorEvents::beginCount: {}", events.beginCount);
			// }
			// if (events.endCount != 0)
			// {
			// 	std::println("b2SensorEvents::endCount: {}", events.endCount);
			// }

			for (int i = 0; i < events.beginCount; ++i)
			{
				const auto& event = events.beginEvents[i];

				const auto sensor_body = b2Shape_GetBody(event.sensorShapeId);
				const auto visitor_body = b2Shape_GetBody(event.visitorShapeId);

				auto* sensor_user_data = b2Body_GetUserData(sensor_body);
				auto* visitor_user_data = b2Body_GetUserData(visitor_body);

				PROMETHEUS_PLATFORM_ASSUME(sensor_user_data != nullptr and visitor_user_data != nullptr);

				auto* sensor_entity = static_cast<Entity*>(sensor_user_data);
				auto* visitor_entity = static_cast<Entity*>(visitor_user_data);

				const auto sensor_type = sensor_entity->type();
				const auto visitor_type = visitor_entity->type();

				std::println("SensorBegin: {} & {}", prometheus::meta::name_of(sensor_type), prometheus::meta::name_of(visitor_type));
			}

			for (int i = 0; i < events.endCount; ++i)
			{
				const auto& event = events.endEvents[i];

				if (not b2Shape_IsValid(event.sensorShapeId) or not b2Shape_IsValid(event.visitorShapeId))
				{
					continue;
				}

				const auto sensor_body = b2Shape_GetBody(event.sensorShapeId);
				const auto visitor_body = b2Shape_GetBody(event.visitorShapeId);

				auto* sensor_user_data = b2Body_GetUserData(sensor_body);
				auto* visitor_user_data = b2Body_GetUserData(visitor_body);

				PROMETHEUS_PLATFORM_ASSUME(sensor_user_data != nullptr and visitor_user_data != nullptr);

				auto* sensor_entity = static_cast<Entity*>(sensor_user_data);
				auto* visitor_entity = static_cast<Entity*>(visitor_user_data);

				const auto sensor_type = sensor_entity->type();
				const auto visitor_type = visitor_entity->type();

				std::println("SensorEnd: {} & {}", prometheus::meta::name_of(sensor_type), prometheus::meta::name_of(visitor_type));
			}
		}
	};

	class ContactProcessor final
	{
	public:
		static auto step(const b2WorldId world) noexcept -> void
		{
			const auto events = b2World_GetContactEvents(world);

			// if (events.beginCount != 0)
			// {
			// 	std::println("b2ContactEvents::beginCount: {}", events.beginCount);
			// }
			// if (events.endCount != 0)
			// {
			// 	std::println("b2ContactEvents::endCount: {}", events.endCount);
			// }
			// if (events.hitCount != 0)
			// {
			// 	std::println("b2ContactEvents::hitCount: {}", events.hitCount);
			// }

			for (int i = 0; i < events.beginCount; ++i)
			{
				const auto& event = events.beginEvents[i];

				const auto body_a = b2Shape_GetBody(event.shapeIdA);
				const auto body_b = b2Shape_GetBody(event.shapeIdB);

				auto* user_data_a = b2Body_GetUserData(body_a);
				auto* user_data_b = b2Body_GetUserData(body_b);

				PROMETHEUS_PLATFORM_ASSUME(user_data_a != nullptr and user_data_b != nullptr);

				auto* entity_a = static_cast<Entity*>(user_data_a);
				auto* entity_b = static_cast<Entity*>(user_data_b);

				const auto type_a = entity_a->type();
				const auto type_b = entity_b->type();

				std::println("ContactBegin: {} & {}", prometheus::meta::name_of(type_a), prometheus::meta::name_of(type_b));

				// 踩中地板
				if (type_a == EntityType::FLOOR or type_b == EntityType::FLOOR)
				{
					const auto handle_projectile_collision = []<typename T>(const FloorEntity* floor, T* entity) noexcept -> void //
								requires std::is_same_v<T, EnemyEntity> or std::is_same_v<T, PlayerEntity>
					{
						// todo: 应用地板的效果
						std::ignore = floor;
						std::ignore = entity;
					};

					if (type_a == EntityType::FLOOR)
					{
						if (type_b == EntityType::ENEMY)
						{
							handle_projectile_collision(static_cast<const FloorEntity*>(entity_a), static_cast<EnemyEntity*>(entity_b));
						}
						else
						{
							PROMETHEUS_PLATFORM_ASSUME(type_b == EntityType::PLAYER);
							handle_projectile_collision(static_cast<const FloorEntity*>(entity_a), static_cast<PlayerEntity*>(entity_b));
						}
					}
					else
					{
						if (type_a == EntityType::ENEMY)
						{
							handle_projectile_collision(static_cast<const FloorEntity*>(entity_b), static_cast<EnemyEntity*>(entity_a));
						}
						else
						{
							PROMETHEUS_PLATFORM_ASSUME(type_a == EntityType::PLAYER);
							handle_projectile_collision(static_cast<const FloorEntity*>(entity_b), static_cast<PlayerEntity*>(entity_a));
						}
					}
				}
				// 飞弹碰撞
				else if (type_a == EntityType::PROJECTILE or type_b == EntityType::PROJECTILE)
				{
					if (type_a == EntityType::PROJECTILE)
					{
						static_cast<ProjectileEntity*>(entity_a)->on_collide(*entity_b);
					}
					else
					{
						static_cast<ProjectileEntity*>(entity_b)->on_collide(*entity_a);
					}
				}
			}

			for (int i = 0; i < events.hitCount; ++i)
			{
				const auto& event = events.hitEvents[i];

				const auto body_a = b2Shape_GetBody(event.shapeIdA);
				const auto body_b = b2Shape_GetBody(event.shapeIdB);

				auto* user_data_a = b2Body_GetUserData(body_a);
				auto* user_data_b = b2Body_GetUserData(body_b);

				PROMETHEUS_PLATFORM_ASSUME(user_data_a != nullptr and user_data_b != nullptr);

				const auto* entity_a = static_cast<Entity*>(user_data_a);
				const auto* entity_b = static_cast<Entity*>(user_data_b);

				const auto type_a = entity_a->type();
				const auto type_b = entity_b->type();

				std::println("ContactHit: {} & {}", prometheus::meta::name_of(type_a), prometheus::meta::name_of(type_b));
			}

			for (int i = 0; i < events.endCount; ++i)
			{
				const auto& event = events.endEvents[i];

				if (not b2Shape_IsValid(event.shapeIdA) or not b2Shape_IsValid(event.shapeIdB))
				{
					continue;
				}

				const auto body_a = b2Shape_GetBody(event.shapeIdA);
				const auto body_b = b2Shape_GetBody(event.shapeIdB);

				auto* user_data_a = b2Body_GetUserData(body_a);
				auto* user_data_b = b2Body_GetUserData(body_b);

				PROMETHEUS_PLATFORM_ASSUME(user_data_a != nullptr and user_data_b != nullptr);

				const auto* entity_a = static_cast<Entity*>(user_data_a);
				const auto* entity_b = static_cast<Entity*>(user_data_b);

				const auto type_a = entity_a->type();
				const auto type_b = entity_b->type();

				std::println("ContactEnd: {} & {}", prometheus::meta::name_of(type_a), prometheus::meta::name_of(type_b));
			}
		}
	};

	// todo: 基于[b2BodyMoveEvent]更新位置,而不是[Entity::update](sync_position_and_rotation)

	class Playground final
	{
	public:
		// 简化代码
		inline static Playground* playground;

	private:
		enum class Movement : std::uint16_t
		{
			STAY,
			FORWARD,
			BACKWARD,
		};

		b2WorldId physics_world_id_;
		ContactProcessor contact_processor_;

		PlayerEntity* player_;
		Movement movement_x_;
		Movement movement_y_;

		std::vector<std::unique_ptr<Entity>> entities_;

		auto create_world() noexcept -> void
		{
			auto def = b2DefaultWorldDef();
			// 无重力世界
			def.gravity = b2Vec2_zero;

			physics_world_id_ = b2CreateWorld(&def);
		}

		auto create_room() noexcept -> void
		{
			constexpr auto wall_thickness = 20.f;
			constexpr auto room_width = static_cast<float>(pd::Constant::window_width - 100);
			constexpr auto room_height = static_cast<float>(pd::Constant::window_height - 100);
			constexpr auto room_x = 50.f;
			constexpr auto room_y = 50.f;

			constexpr auto top_position = sf::Vector2f{room_x + room_width / 2, room_y - wall_thickness / 2};
			constexpr auto top_size = sf::Vector2f{room_width, wall_thickness};
			constexpr auto bottom_position = sf::Vector2f{room_x + room_width / 2, room_y + room_height + wall_thickness / 2};
			constexpr auto bottom_size = sf::Vector2f{room_width, wall_thickness};
			constexpr auto left_position = sf::Vector2f{room_x - wall_thickness / 2, room_y + room_height / 2};
			constexpr auto left_size = sf::Vector2f{wall_thickness, room_height};
			constexpr auto right_position = sf::Vector2f{room_x + room_width + wall_thickness / 2, room_y + room_height / 2};
			constexpr auto right_size = sf::Vector2f{wall_thickness, room_height};

			auto top_wall = std::make_unique<WallEntity>(physics_world_id_, top_position, top_size);
			auto bottom_wall = std::make_unique<WallEntity>(physics_world_id_, bottom_position, bottom_size);
			auto left_wall = std::make_unique<WallEntity>(physics_world_id_, left_position, left_size);
			auto right_wall = std::make_unique<WallEntity>(physics_world_id_, right_position, right_size);

			entities_.emplace_back(std::move(top_wall));
			entities_.emplace_back(std::move(bottom_wall));
			entities_.emplace_back(std::move(left_wall));
			entities_.emplace_back(std::move(right_wall));
		}

		auto create_floor() noexcept -> void
		{
			constexpr auto size = sf::Vector2f{50, 50};
			constexpr std::array normal_positions
			{
					sf::Vector2f{400 + 0 * size.x, 150},
					sf::Vector2f{400 + 1 * size.x, 150},
					sf::Vector2f{400 + 2 * size.x, 150},
					sf::Vector2f{400 + 3 * size.x, 150},
					sf::Vector2f{400 + 4 * size.x, 150},
					sf::Vector2f{400 + 5 * size.x, 150},
					sf::Vector2f{400 + 6 * size.x, 150},
					sf::Vector2f{400 + 7 * size.x, 150},
					sf::Vector2f{400 + 8 * size.x, 150},
					sf::Vector2f{400 + 9 * size.x, 150},
			};
			constexpr std::array ice_positions
			{
					sf::Vector2f{400 + 0 * size.x, 250},
					sf::Vector2f{400 + 1 * size.x, 250},
					sf::Vector2f{400 + 2 * size.x, 250},
					sf::Vector2f{400 + 3 * size.x, 250},
					sf::Vector2f{400 + 4 * size.x, 250},
					sf::Vector2f{400 + 5 * size.x, 250},
					sf::Vector2f{400 + 6 * size.x, 250},
					sf::Vector2f{400 + 7 * size.x, 250},
					sf::Vector2f{400 + 8 * size.x, 250},
					sf::Vector2f{400 + 9 * size.x, 250},
			};
			constexpr std::array mud_positions
			{
					sf::Vector2f{400 + 0 * size.x, 350},
					sf::Vector2f{400 + 1 * size.x, 350},
					sf::Vector2f{400 + 2 * size.x, 350},
					sf::Vector2f{400 + 3 * size.x, 350},
					sf::Vector2f{400 + 4 * size.x, 350},
					sf::Vector2f{400 + 5 * size.x, 350},
					sf::Vector2f{400 + 6 * size.x, 350},
					sf::Vector2f{400 + 7 * size.x, 350},
					sf::Vector2f{400 + 8 * size.x, 350},
					sf::Vector2f{400 + 9 * size.x, 350},
			};

			auto do_create = [this, size](const auto& positions, const FloorEntity::Type type) noexcept -> void
			{
				for (const auto position: positions)
				{
					auto entity = std::make_unique<FloorEntity>(physics_world_id_, position, size, type);
					entities_.emplace_back(std::move(entity));
				}
			};

			do_create(normal_positions, FloorEntity::Type::NORMAL);
			do_create(ice_positions, FloorEntity::Type::ICE);
			do_create(mud_positions, FloorEntity::Type::MUD);
		}

		auto create_decoration() noexcept -> void
		{
			constexpr auto size = sf::Vector2f{50, 50};
			constexpr std::array positions
			{
					// 左
					sf::Vector2f{1200, 150 + 0 * (2 * size.y)},
					sf::Vector2f{1200, 150 + 1 * (2 * size.y)},
					sf::Vector2f{1200, 150 + 2 * (2 * size.y)},
					sf::Vector2f{1200, 150 + 3 * (2 * size.y)},
					sf::Vector2f{1200, 150 + 4 * (2 * size.y)},
					sf::Vector2f{1200, 150 + 5 * (2 * size.y)},
					// 右
					sf::Vector2f{1400, 200 + 0 * (2 * size.y)},
					sf::Vector2f{1400, 200 + 1 * (2 * size.y)},
					sf::Vector2f{1400, 200 + 2 * (2 * size.y)},
					sf::Vector2f{1400, 200 + 3 * (2 * size.y)},
					sf::Vector2f{1400, 200 + 4 * (2 * size.y)},
					sf::Vector2f{1400, 200 + 5 * (2 * size.y)},
			};

			for (const auto position: positions)
			{
				// todo: 障碍物大小
				auto decoration = std::make_unique<DecorationEntity>(physics_world_id_, position, size);
				entities_.emplace_back(std::move(decoration));
			}
		}

		auto create_player() noexcept -> void
		{
			auto player = std::make_unique<PlayerEntity>(
				physics_world_id_,
				// 房间中心
				sf::Vector2f{pd::Constant::window_width / 2.f, pd::Constant::window_height / 2.f},
				// todo: 玩家大小
				sf::Vector2f{35, 35},
				// todo: 玩家最大速度
				300.f
			);

			player_ = player.get();
			entities_.emplace_back(std::move(player));
		}

	public:
		Playground() noexcept
			:
			physics_world_id_{b2_nullWorldId},
			player_{nullptr},
			movement_x_{Movement::STAY},
			movement_y_{Movement::STAY}
		{
			// 创建世界
			create_world();

			// 创建房间墙壁
			create_room();

			// 创建地板
			create_floor();

			// 创建地形/装饰物
			create_decoration();

			// 创建玩家
			create_player();
		}

		auto handle_event(const sf::Event& event) noexcept -> void
		{
			if (const auto* kp = event.getIf<sf::Event::KeyPressed>())
			{
				using sf::Keyboard::Key;
				if (kp->code == Key::A)
				{
					movement_x_ = Movement::BACKWARD;
				}
				if (kp->code == Key::D)
				{
					movement_x_ = Movement::FORWARD;
				}
				if (kp->code == Key::W)
				{
					movement_y_ = Movement::BACKWARD;
				}
				if (kp->code == Key::S)
				{
					movement_y_ = Movement::FORWARD;
				}

				return;
			}

			if (const auto* kr = event.getIf<sf::Event::KeyReleased>())
			{
				using sf::Keyboard::Key;
				if (kr->code == Key::A)
				{
					movement_x_ = Movement::STAY;
				}
				if (kr->code == Key::D)
				{
					movement_x_ = Movement::STAY;
				}
				if (kr->code == Key::W)
				{
					movement_y_ = Movement::STAY;
				}
				if (kr->code == Key::S)
				{
					movement_y_ = Movement::STAY;
				}

				return;
			}

			if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>())
			{
				if (mbp->button == sf::Mouse::Button::Left)
				{
					spawn_projectile_to<BaseProjectileEntity>(sf::Vector2f{mbp->position});
				}
				else if (mbp->button == sf::Mouse::Button::Right)
				{
					spawn_projectile_to<BounceProjectileEntity>(sf::Vector2f{mbp->position});
				}

				return;
			}

			//
		}

		auto update(const float delta) noexcept -> void
		{
			// 移除非活跃实体
			{
				auto r = std::ranges::remove_if(
					entities_,
					[&](const auto& entity) noexcept -> bool
					{
						return not entity->is_active();
					}
				);
				entities_.erase(r.begin(), entities_.end());
			}

			// 处理玩家输入
			{
				b2Vec2 force{0, 0};

				if (movement_x_ == Movement::FORWARD)
				{
					force.x = 1;
				}
				else if (movement_x_ == Movement::BACKWARD)
				{
					force.x = -1;
				}

				if (movement_y_ == Movement::FORWARD)
				{
					force.y = 1;
				}
				else if (movement_y_ == Movement::BACKWARD)
				{
					force.y = -1;
				}

				player_->apply_force(force);
			}

			// 更新物理世界
			b2World_Step(physics_world_id_, delta, 4);

			// 处理物理世界事件
			SensorProcessor::step(physics_world_id_);
			ContactProcessor::step(physics_world_id_);

			// 更新所有实体
			for (const auto& entity: entities_)
			{
				entity->update(delta);
			}
		}

		auto render(sf::RenderWindow& window) noexcept -> void
		{
			// 按渲染层排序
			std::ranges::sort(entities_, std::ranges::less{}, &Entity::render_layer);

			// 渲染所有实体
			for (const auto& entity: entities_)
			{
				entity->render(window);
			}
		}

		auto show_info() noexcept -> void
		{
			ImGui::Begin("物理调试信息", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			// 显示实体数量
			ImGui::Text("实体数量: %zu", entities_.size());

			// 显示玩家信息
			{
				const auto position = player_->position();
				const auto velocity = player_->velocity();

				ImGui::Text("Position: (%.1f, %.1f), Velocity: (%.1f, %.1f)[%.1f]", position.x, position.y, velocity.x, velocity.y, velocity.length());
			}

			// 显示敌人信息
			{
				const auto count = std::ranges::count(entities_, EntityType::ENEMY, &Entity::type);
				ImGui::Text("活动敌人: %td", count);

				// ReSharper disable once CppLocalVariableMayBeConst
				auto enemies =
						entities_ |
						std::views::transform(
							[](const auto& entity) noexcept -> EnemyEntity*
							{
								if (const auto type = entity->type();
									type == EntityType::ENEMY)
								{
									return static_cast<EnemyEntity*>(entity.get());
								}

								return nullptr;
							}
						) |
						std::views::filter(std::identity{});
				for (const auto& enemy: enemies)
				{
					const auto position = enemy->position();
					const auto velocity = enemy->velocity();

					ImGui::Text("Position: (%.1f, %.1f), Velocity: (%.1f, %.1f)[%.1f]", position.x, position.y, velocity.x, velocity.y, velocity.length());
				}
			}

			// 显示飞弹信息
			{
				const auto count = std::ranges::count(entities_, EntityType::PROJECTILE, &Entity::type);
				ImGui::Text("活动飞弹: %td", count);

				// ReSharper disable once CppLocalVariableMayBeConst
				auto projectiles =
						entities_ |
						std::views::transform(
							[](const auto& entity) noexcept -> ProjectileEntity*
							{
								if (const auto type = entity->type(); type == EntityType::PROJECTILE)
								{
									return static_cast<ProjectileEntity*>(entity.get());
								}

								return nullptr;
							}
						) |
						std::views::filter(std::identity{});
				for (const auto projectile: projectiles)
				{
					const auto position = projectile->position();
					const auto velocity = projectile->velocity();

					ImGui::Text("Position: (%.1f, %.1f), Velocity: (%.1f, %.1f)[%.1f]", position.x, position.y, velocity.x, velocity.y, velocity.length());
				}
			}

			ImGui::End();
		}

		// 往目标方向发射
		template<std::derived_from<ProjectileEntity> T>
		auto spawn_projectile_toward(const sf::Vector2f position, const sf::Vector2f direction) noexcept -> void
		{
			const auto player_size = player_->size();

			const auto direction_normalized = direction.normalized();
			// 偏移一定要足够,避免发射时直接碰撞本体
			const auto offset = sf::Vector2f{direction_normalized.x * player_size.x, direction_normalized.y * player_size.y};
			const auto spawn_position = position + offset;

			auto projectile = [&] noexcept -> std::unique_ptr<T>
			{
				if constexpr (std::is_same_v<T, BaseProjectileEntity>)
				{
					return std::make_unique<BaseProjectileEntity>(
						physics_world_id_,
						EntityType::PLAYER,
						spawn_position,
						// todo: 飞弹半径
						10.f,
						// todo: 飞弹速度
						direction_normalized * 500.f,
						// 存在5秒
						5.f
					);
				}
				else if constexpr (std::is_same_v<T, BounceProjectileEntity>)
				{
					return std::make_unique<BounceProjectileEntity>(
						physics_world_id_,
						EntityType::PLAYER,
						spawn_position,
						// todo: 飞弹半径
						10.f,
						// todo: 飞弹速度
						direction_normalized * 500.f,
						// 存在5秒
						5.f,
						// 弹跳3次
						3
					);
				}
				else
				{
					PROMETHEUS_COMPILER_UNREACHABLE();
				}
			}();

			entities_.emplace_back(std::move(projectile));
		}

		// 往目标位置发射
		template<std::derived_from<ProjectileEntity> T>
		auto spawn_projectile_to(const sf::Vector2f target_position) noexcept -> void
		{
			const auto player_position = player_->position();
			const auto direction = target_position - player_position;

			spawn_projectile_toward<T>(player_position, direction);
		}
	};
}
