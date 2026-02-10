// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <algorithm>
#include <ranges>
#include <print>

#include <box2d/box2d.h>

#include <SFML/Graphics.hpp>

#include <imgui.h>

#include <pd/defines.hpp>
#include <pd/constants.hpp>
#include <pd/utility/enumeration.hpp>

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

		PD_MAGIC_ENUM_FLAG
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
		b2Body* body_;

		bool active_;
		RenderLayer render_layer_;

	public:
		explicit Entity(const RenderLayer render_layer) noexcept
			: body_{nullptr},
			  active_{true},
			  render_layer_{render_layer} {}

		Entity(const Entity&) noexcept = delete;
		auto operator=(const Entity&) noexcept -> Entity& = delete;

		Entity(Entity&&) noexcept = default;
		auto operator=(Entity&&) noexcept -> Entity& = default;

		virtual ~Entity() noexcept
		{
			// 避免ContactListener::EndContact出问题
			body_->GetUserData().pointer = 0;

			auto* world = body_->GetWorld();
			world->DestroyBody(body_);
		}

		[[nodiscard]] virtual auto type() const noexcept -> EntityType = 0;

		virtual auto update(float delta) noexcept -> void = 0;

		virtual auto render(sf::RenderWindow& window) noexcept -> void = 0;

		// 这个接口本不应该,不过简化逻辑
		[[nodiscard]] auto body() const noexcept -> b2Body*
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
		[[nodiscard]] static auto create_physics(b2World& world, const sf::Vector2f position, const sf::Vector2f size, const EntityType type) noexcept -> b2Body*
		{
			b2Body* body;
			// 创建BODY
			{
				const auto physics_position = pd::Constant::to_physics(position);

				b2BodyDef def{};
				def.type = b2_dynamicBody;
				def.position = physics_position;
				// todo: 需不需要可以旋转?
				def.fixedRotation = true;
				def.linearDamping = 5;

				body = world.CreateBody(&def);
			}
			// 创建FIXTURE
			{
				const auto physics_size = 0.5f * pd::Constant::to_physics(size);

				b2PolygonShape shape{};
				shape.SetAsBox(physics_size.x, physics_size.y);

				b2FixtureDef def{};
				def.shape = &shape;
				def.density = 1;
				def.friction = 0.3f;
				def.restitution = 0.05f;
				def.filter.categoryBits = static_cast<uint16>(type);
				// 可以和墙壁/地板/装饰物/敌人/玩家/飞弹碰撞
				def.filter.maskBits = static_cast<uint16>(
					EntityType::WALL | EntityType::FLOOR | EntityType::DECORATION |
					EntityType::ENEMY | EntityType::PLAYER |
					EntityType::PROJECTILE
				);

				body->CreateFixture(&def);
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
			auto physics_velocity = body_->GetLinearVelocity();

			if (const auto physics_speed_squared = physics_velocity.LengthSquared();
				physics_speed_squared > move_speed_current_physics_max_squared_)
			{
				const auto physics_speed = std::sqrt(physics_speed_squared);

				physics_velocity *= move_speed_current_physics_max_ / physics_speed;
				body_->SetLinearVelocity(physics_velocity);
			}
		}

		auto sync_position_and_rotation() noexcept -> void
		{
			const auto position = body_->GetPosition();
			const auto radians = body_->GetAngle();

			shape_.setPosition(pd::Constant::from_physics(position));
			shape_.setRotation(sf::radians(radians));
		}

	public:
		ActorEntity(
			b2World& world,
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
			body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

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
			const auto physics_velocity = body_->GetLinearVelocity();
			return pd::Constant::from_physics(physics_velocity);
		}

		auto apply_force(b2Vec2 force) const noexcept -> void
		{
			if (force == b2Vec2_zero)
			{
				return;
			}

			force.Normalize();

			force *= body_->GetMass() * move_speed_current_max_;
			body_->ApplyForceToCenter(force, true);
		}
	};

	// 敌人
	class EnemyEntity final : public ActorEntity
	{
	public:
		EnemyEntity(
			b2World& world,
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
			b2World& world,
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
		[[nodiscard]] static auto create_physics(b2World& world, const sf::Vector2f position, const sf::Vector2f size) noexcept -> b2Body*
		{
			b2Body* body;
			// 创建BODY
			{
				const auto physics_position = pd::Constant::to_physics(position);

				b2BodyDef def{};
				def.type = b2_staticBody;
				def.position = physics_position;
				def.fixedRotation = true;

				body = world.CreateBody(&def);
			}
			// 创建FIXTURE
			{
				const auto physics_size = 0.5f * pd::Constant::to_physics(size);

				b2PolygonShape shape{};
				shape.SetAsBox(physics_size.x, physics_size.y);

				b2FixtureDef def{};
				def.shape = &shape;
				def.density = 0;
				def.friction = 0.5f;
				def.restitution = 0.7f;
				def.filter.categoryBits = static_cast<uint16>(EntityType::WALL);
				// 可以和敌人/玩家/飞弹碰撞
				def.filter.maskBits = static_cast<uint16>(
					EntityType::ENEMY | EntityType::PLAYER |
					EntityType::PROJECTILE
				);

				body->CreateFixture(&def);
			}

			return body;
		}

		sf::RectangleShape shape_;

	public:
		WallEntity(
			b2World& world,
			const sf::Vector2f position,
			const sf::Vector2f size
		) noexcept
			: Entity{RenderLayer::WALL},
			  shape_{size}
		{
			// 创建body
			body_ = create_physics(world, position, size);

			// 设置UserData
			body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

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
		[[nodiscard]] static auto create_physics(b2World& world, const sf::Vector2f position, const sf::Vector2f size) noexcept -> b2Body*
		{
			b2Body* body;
			// 创建BODY
			{
				const auto physics_position = pd::Constant::to_physics(position);

				b2BodyDef def{};
				def.type = b2_staticBody;
				def.position = physics_position;
				def.fixedRotation = true;
				def.linearDamping = 5;

				body = world.CreateBody(&def);
			}
			// 创建FIXTURE
			{
				const auto physics_size = 0.5f * pd::Constant::to_physics(size);

				b2PolygonShape shape{};
				shape.SetAsBox(physics_size.x, physics_size.y);

				b2FixtureDef def{};
				def.shape = &shape;
				// 传感器不阻止移动,可以检测接触
				def.isSensor = true;
				def.filter.categoryBits = static_cast<uint16>(EntityType::FLOOR);
				// 可以和敌人/玩家碰撞
				def.filter.maskBits = static_cast<uint16>(EntityType::ENEMY | EntityType::PLAYER);

				body->CreateFixture(&def);
			}

			return body;
		}

		sf::RectangleShape shape_;

	public:
		FloorEntity(
			b2World& world,
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
			body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

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
		[[nodiscard]] static auto create_physics(b2World& world, const sf::Vector2f position, const sf::Vector2f size) noexcept -> b2Body*
		{
			b2Body* body;
			// 创建BODY
			{
				const auto physics_position = pd::Constant::to_physics(position);

				b2BodyDef def{};
				def.type = b2_staticBody;
				def.position = physics_position;
				def.fixedRotation = true;

				body = world.CreateBody(&def);
			}
			// 创建FIXTURE
			{
				const auto physics_size = 0.5f * pd::Constant::to_physics(size);

				b2PolygonShape shape{};
				shape.SetAsBox(physics_size.x, physics_size.y);

				b2FixtureDef def{};
				def.shape = &shape;
				def.density = 0;
				def.friction = 0.5f;
				def.restitution = 0.7f;
				def.filter.categoryBits = static_cast<uint16>(EntityType::DECORATION);
				// 可以和敌人/玩家/飞弹碰撞
				// todo: 如果是一个可以移动的装饰物呢?如果可以移动,则还可以和墙壁/装饰物碰撞
				def.filter.maskBits = static_cast<uint16>(
					EntityType::ENEMY | EntityType::PLAYER |
					EntityType::PROJECTILE
				);

				body->CreateFixture(&def);
			}

			return body;
		}

		sf::RectangleShape shape_;

	public:
		DecorationEntity(
			b2World& world,
			const sf::Vector2f position,
			const sf::Vector2f size
		) noexcept
			: Entity{RenderLayer::DECORATION},
			  shape_{size}
		{
			// 创建body
			body_ = create_physics(world, position, size);

			// 设置UserData
			body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

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
			b2World& world,
			const EntityType owner,
			const sf::Vector2f position,
			const float radius,
			const sf::Vector2f velocity
		) noexcept -> b2Body*
		{
			b2Body* body;
			// 创建BODY
			{
				const auto physics_position = pd::Constant::to_physics(position);
				const auto physics_velocity = pd::Constant::to_physics(velocity);

				b2BodyDef def{};
				def.type = b2_dynamicBody;
				def.position = physics_position;
				def.linearVelocity = physics_velocity;
				def.linearDamping = 0.05f;
				def.fixedRotation = false;
				def.bullet = true;

				body = world.CreateBody(&def);
			}
			// 创建FIXTURE
			{
				const auto physics_radius = pd::Constant::to_physics(radius);

				b2CircleShape shape{};
				shape.m_radius = physics_radius;

				b2FixtureDef def{};
				def.shape = &shape;
				def.density = 0.5f;
				def.friction = 0;
				def.restitution = 0.8f;
				def.filter.categoryBits = static_cast<uint16>(EntityType::PROJECTILE);
				// 可以和墙壁/装饰物/敌人/玩家/碰撞
				def.filter.maskBits = static_cast<uint16>(
					EntityType::WALL | EntityType::DECORATION |
					(owner == EntityType::ENEMY ? EntityType::PLAYER : EntityType::ENEMY)
				);

				body->CreateFixture(&def);
			}

			return body;
		}

		// 主体
		sf::CircleShape shape_;
		// 拖尾效果
		sf::ConvexShape tail_;

		auto sync_position_and_rotation() noexcept -> void
		{
			const auto position = body_->GetPosition();
			const auto radians = body_->GetAngle();

			shape_.setPosition(pd::Constant::from_physics(position));
			shape_.setRotation(sf::radians(radians));
		}

	public:
		BaseProjectileEntity(
			b2World& world,
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
			body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

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
			if (const auto velocity = body_->GetLinearVelocity();
				velocity.LengthSquared() < min_velocity_2)
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

			auto physics_velocity = body_->GetLinearVelocity();
			physics_velocity.Normalize();

			const auto direction = sf::Vector2f{physics_velocity.x, physics_velocity.y};
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
			const auto physics_velocity = body_->GetLinearVelocity();
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
			b2World& world,
			const EntityType owner,
			const sf::Vector2f position,
			const float radius,
			const sf::Vector2f velocity
		) noexcept -> b2Body*
		{
			b2Body* body;
			// 创建BODY
			{
				const auto physics_position = pd::Constant::to_physics(position);
				const auto physics_velocity = pd::Constant::to_physics(velocity);

				b2BodyDef def{};
				def.type = b2_dynamicBody;
				def.position = physics_position;
				def.linearVelocity = physics_velocity;
				def.linearDamping = 0.05f;
				def.fixedRotation = false;
				def.bullet = true;

				body = world.CreateBody(&def);
			}
			// 创建FIXTURE
			{
				const auto physics_radius = pd::Constant::to_physics(radius);

				b2CircleShape shape{};
				shape.m_radius = physics_radius;

				b2FixtureDef def{};
				def.shape = &shape;
				def.density = 0.5f;
				def.friction = 0;
				def.restitution = 0.8f;
				def.filter.categoryBits = static_cast<uint16>(EntityType::PROJECTILE);
				// 可以和墙壁/装饰物/敌人/玩家/碰撞
				def.filter.maskBits = static_cast<uint16>(
					EntityType::WALL | EntityType::DECORATION |
					(owner == EntityType::ENEMY ? EntityType::PLAYER : EntityType::ENEMY)
				);

				body->CreateFixture(&def);
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
			const auto position = body_->GetPosition();
			const auto radians = body_->GetAngle();

			shape_.setPosition(pd::Constant::from_physics(position));
			shape_.setRotation(sf::radians(radians));
		}

	public:
		BounceProjectileEntity(
			b2World& world,
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
			body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

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
			if (const auto velocity = body_->GetLinearVelocity();
				velocity.LengthSquared() < min_velocity_2)
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

			auto physics_velocity = body_->GetLinearVelocity();
			physics_velocity.Normalize();

			const auto direction = sf::Vector2f{physics_velocity.x, physics_velocity.y};
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
			for (auto* f = body_->GetFixtureList(); f; f = f->GetNext())
			{
				// todo: 弹性减少系数
				constexpr auto restitution_factor = 0.9f;
				const auto restitution = f->GetRestitution();
				f->SetRestitution(restitution * restitution_factor);
			}

			// 每次反弹后稍微减少速度
			auto velocity = body_->GetLinearVelocity();
			// todo: 减速系数
			// 减速系数可以随着反弹次数增加而增大
			constexpr auto slowdown_factor_default = 0.95f;
			auto slowdown_factor = slowdown_factor_default - (bounces_current_ * 0.05f);
			// 不低于0.3倍
			slowdown_factor = std::ranges::max(slowdown_factor, 0.3f);
			// 设置速度
			velocity *= slowdown_factor;
			body_->SetLinearVelocity(std::ranges::max(slowdown_factor, 0.3f) * velocity);
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
			const auto physics_velocity = body_->GetLinearVelocity();
			return pd::Constant::from_physics(physics_velocity);
		}
	};

	class ChainProjectileEntity final : public ProjectileEntity
	{
	public:
		// ReSharper disable once IdentifierTypo
		constexpr static auto npos = std::numeric_limits<std::size_t>::max();

	private:
		[[nodiscard]] static auto create_shapes(const std::span<const float> segment_radii) noexcept -> std::vector<sf::CircleShape>
		{
			std::vector<sf::CircleShape> shapes{};
			shapes.reserve(segment_radii.size());

			std::ranges::transform(
				segment_radii,
				std::back_inserter(shapes),
				[](const float radius) noexcept -> sf::CircleShape
				{
					return sf::CircleShape{radius};
				}
			);

			return shapes;
		}

		[[nodiscard]] static auto create_segment(
			b2World& world,
			const EntityType owner,
			const b2Vec2 physics_position,
			const float physics_radius,
			const float mass_multiplier = 1.f
		) noexcept -> b2Body*
		{
			b2Body* body;
			// 创建BODY
			{
				b2BodyDef def{};
				def.type = b2_dynamicBody;
				def.position = physics_position;
				def.linearDamping = 0.1f;
				def.angularDamping = 0.5f;

				body = world.CreateBody(&def);
			}
			// 创建FIXTURE
			{
				b2CircleShape shape{};
				shape.m_radius = physics_radius;

				b2FixtureDef def{};
				def.shape = &shape;
				def.density = 0.5f * mass_multiplier;
				def.friction = 0.1f;
				def.restitution = 0.6f;
				def.filter.categoryBits = static_cast<uint16>(EntityType::PROJECTILE);
				// 可以和墙壁/装饰物/敌人/玩家/碰撞
				def.filter.maskBits = static_cast<uint16>(
					EntityType::WALL | EntityType::DECORATION |
					(owner == EntityType::ENEMY ? EntityType::PLAYER : EntityType::ENEMY)
				);

				body->CreateFixture(&def);
			}

			return body;
		}

		[[nodiscard]] static auto create_segments(
			b2World& world,
			const EntityType owner,
			sf::Vector2f position,
			const std::span<const float> segment_radii,
			const sf::Vector2f velocity
		) noexcept -> std::vector<b2Body*>
		{
			std::vector<b2Body*> segments{};
			segments.reserve(segment_radii.size());

			const auto direction = velocity.normalized();
			const auto total_length = std::ranges::fold_left(
				segment_radii,
				0.f,
				[](const float sum, const float r) noexcept -> float
				{
					return sum + r * 2;
				}
			);

			position += direction * (total_length * 1.5f);

			// 头部 - 在中心位置
			{
				const auto radius = segment_radii[0];

				const auto physics_position = pd::Constant::to_physics(position);
				const auto physics_radius = pd::Constant::to_physics(radius);
				const auto physics_velocity = pd::Constant::to_physics(velocity);

				auto* segment = create_segment(world, owner, physics_position, physics_radius, 2);
				segment->SetLinearVelocity(physics_velocity);
				segments.emplace_back(segment);
			}
			// 其他segment - 按弧线排列
			{
				const auto segment_count = static_cast<float>(segment_radii.size());

				const auto perpendicular = sf::Vector2f{-direction.y, direction.x};

				for (std::size_t i = 1; i < segment_radii.size(); ++i)
				{
					constexpr auto arc_angle = 30.f;

					const auto this_radius = segment_radii[i];
					const auto this_physics_radius = pd::Constant::to_physics(this_radius);

					// 计算弧线上的位置
					const float angle_ratio = static_cast<float>(i - 1) / (segment_count - 2);
					// 在弧线上均匀分布
					const float angle = arc_angle * (angle_ratio - 0.5f);
					// 旋转方向向量
					// const auto rotated_direction = direction.rotatedBy(sf::degrees(angle));
					// 计算偏移距离,让整个链看起来更像蛇形
					// const float offset_distance = this_radius * 2 + 5.f;
					// const auto offset = rotated_direction * offset_distance;

					// 从头部位置向后偏移
					const auto this_position = position - direction * (this_radius * i * 0.8f) + perpendicular * (angle_ratio - 0.5f) * (this_radius * 3);
					const auto this_physics_position = pd::Constant::to_physics(this_position);

					auto* segment = create_segment(world, owner, this_physics_position, this_physics_radius);
					// 给每个segment不同的初始速度和方向,形成自然摆动
					const auto segment_velocity = velocity.rotatedBy(sf::degrees(angle * 0.5f)) * (0.85f - i * 0.1f);
					segment->SetLinearVelocity(pd::Constant::to_physics(segment_velocity));

					segments.emplace_back(segment);
				}
			}

			return segments;
		}

		[[nodiscard]] static auto create_joint(
			b2World& world,
			b2Body& body_a,
			b2Body& body_b,
			const float physics_length,
			const float break_force
		) noexcept -> b2DistanceJoint*
		{
			b2DistanceJointDef def{};
			def.Initialize(&body_a, &body_b, body_a.GetWorldCenter(), body_b.GetWorldCenter());
			def.collideConnected = false;
			def.length = physics_length;

			auto* joint = world.CreateJoint(&def);
			// joint储存自身断裂的力量
			joint->GetUserData().pointer = std::bit_cast<std::uint32_t>(break_force);

			return static_cast<b2DistanceJoint*>(joint);
		}

		[[nodiscard]] static auto create_joints(
			b2World& world,
			const std::span<const float> segment_radii,
			const std::span<const float> break_forces,
			const std::span<b2Body*>& segments
		) noexcept -> std::vector<b2DistanceJoint*>
		{
			PD_ASSERT(segment_radii.size() == break_forces.size());
			PD_ASSERT(segment_radii.size() == segments.size());

			std::vector<b2DistanceJoint*> joints{};
			joints.reserve(segment_radii.size() - 1);

			for (std::size_t i = 0; i < segment_radii.size() - 1; ++i)
			{
				const auto radius = segment_radii[i];
				const auto next_radius = segment_radii[i + 1];
				const auto joint_length = radius + next_radius + 2;

				const auto break_force = break_forces[i];

				auto* segment = segments[i];
				auto* next_segment = segments[i + 1];

				const auto physics_joint_length = pd::Constant::to_physics(joint_length);

				auto* joint = create_joint(world, *segment, *next_segment, physics_joint_length, break_force);
				joints.emplace_back(joint);
			}

			return joints;
		}

		[[nodiscard]] static auto create_physics(
			b2World& world,
			const EntityType owner,
			const sf::Vector2f position,
			const std::span<const float> segment_radii,
			const sf::Vector2f velocity,
			const std::span<const float> break_force,
			std::vector<sf::CircleShape>& out_shapes,
			std::vector<b2Body*>& out_segments,
			std::vector<b2DistanceJoint*>& out_joints
		) noexcept -> b2Body*
		{
			out_shapes = create_shapes(segment_radii);
			out_segments = create_segments(world, owner, position, segment_radii, velocity);
			out_joints = create_joints(world, segment_radii, break_force, out_segments);

			// 使用第一个segment作为主body
			return out_segments.front();
		}

		std::vector<sf::CircleShape> shapes_;
		std::vector<b2Body*> segments_;
		std::vector<b2DistanceJoint*> joints_;
		// 断裂位置
		std::size_t broken_joint_index_;

		auto split_on_broken() noexcept -> void;

		auto sync_position_and_rotation() noexcept -> void
		{
			for (auto [shape, segment]: std::views::zip(shapes_, segments_))
			{
				const auto position = body_->GetPosition();
				const auto radians = body_->GetAngle();

				shape.setPosition(pd::Constant::from_physics(position));
				shape.setRotation(sf::radians(radians));
			}
		}

	public:
		ChainProjectileEntity(
			b2World& world,
			const EntityType owner,
			const sf::Vector2f position,
			const std::span<float> segment_radii,
			const sf::Vector2f velocity,
			const std::span<const float> break_force,
			const float lifetime
		) noexcept
			: ProjectileEntity{lifetime},
			  broken_joint_index_{npos}
		{
			// 创建body
			body_ = create_physics(world, owner, position, segment_radii, velocity, break_force, shapes_, segments_, joints_);

			// 设置UserData
			std::ranges::for_each(
				segments_,
				[p = reinterpret_cast<uintptr_t>(this)](auto* body) noexcept -> void
				{
					body->GetUserData().pointer = p;
				}
			);

			std::ranges::for_each(
				shapes_,
				[](auto& shape) noexcept -> void
				{
					// 飞弹颜色
					shape.setFillColor(sf::Color::Red);
					// 中心对齐
					shape.setOrigin({shape.getRadius(), shape.getRadius()});
					// 不需要初始化位置(因为我们会在update中更新位置)
					// 更何况这里也不方便直接拿到位置 :)
				}
			);
		}

		~ChainProjectileEntity() noexcept override
		{
			// 销毁所有关节
			std::ranges::for_each(
				joints_,
				[](auto* joint) noexcept -> void
				{
					auto* body_a = joint->GetBodyA();
					auto* world = body_a->GetWorld();

					world->DestroyJoint(joint);
				}
			);

			// 销毁所有segment的body(头部由基类析构销毁)
			std::ranges::for_each(
				segments_ | std::views::drop(1),
				[](auto& segment) noexcept -> void
				{
					// 避免ContactListener出问题
					segment->GetUserData().pointer = 0;

					auto* world = segment->GetWorld();
					world->DestroyBody(segment);
				}
			);
		}

		auto update(const float delta) noexcept -> void override
		{
			ProjectileEntity::update(delta);

			if (not active_)
			{
				return;
			}

			// 如果已经断开(例如第一个节点与其他物体碰撞)
			if (broken_joint_index_ != npos)
			{
				split_on_broken();
				return;
			}

			// 检查关节是否断裂
			for (std::size_t i = 0; i < joints_.size(); ++i)
			{
				auto* joint = joints_[i];

				// 检查关节受力
				const auto force = joint->GetReactionForce(1.f / delta);
				const auto force_magnitude = force.Length();

				if (const auto break_force = std::bit_cast<float>(static_cast<std::uint32_t>(joint->GetUserData().pointer));
					force_magnitude > break_force)
				{
					// 记录断裂位置
					broken_joint_index_ = i;

					// 直接结束
					break;
				}
			}

			// 关节受力断开
			if (broken_joint_index_ != npos)
			{
				split_on_broken();
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

			std::ranges::for_each(
				shapes_,
				[&](const auto& shape) noexcept -> void
				{
					window.draw(shape);
				}
			);
		}

		// 处理碰撞
		auto on_collide(Entity& entity) noexcept -> void override
		{
			ProjectileEntity::on_collide(entity);

			// 第一个节点与其他物体碰撞
			broken_joint_index_ = 0;

			// 不要标记为不活跃,因为其会等到下一次update时才进行分裂
			// active_ = false;
		}

		auto position() const noexcept -> sf::Vector2f override
		{
			const auto& shape = shapes_[0];
			return shape.getPosition();
		}

		[[nodiscard]] auto size() const noexcept -> sf::Vector2f override
		{
			const auto& shape = shapes_[0];
			return {shape.getRadius(), shape.getRadius()};
		}

		[[nodiscard]] auto velocity() const noexcept -> sf::Vector2f override
		{
			const auto physics_velocity = body_->GetLinearVelocity();
			return pd::Constant::from_physics(physics_velocity);
		}
	};

	// todo: 其他类型飞弹
	// 可以直接穿过地形/装饰物的飞弹?
	// 范围性飞弹?

	// 碰撞监听器
	class ContactListener final : public b2ContactListener
	{
	public:
		using list_type = std::vector<Entity*>;

	private:
		list_type inactive_entities_;

	public:
		void BeginContact(b2Contact* contact) override
		{
			const auto* fixture_a = contact->GetFixtureA();
			const auto* fixture_b = contact->GetFixtureB();

			const auto* body_a = fixture_a->GetBody();
			const auto* body_b = fixture_b->GetBody();

			auto* entity_a = reinterpret_cast<Entity*>(body_a->GetUserData().pointer); // NOLINT(performance-no-int-to-ptr)
			auto* entity_b = reinterpret_cast<Entity*>(body_b->GetUserData().pointer); // NOLINT(performance-no-int-to-ptr)

			PD_ASSERT(entity_a != nullptr and entity_b != nullptr);

			const auto type_a = entity_a->type();
			const auto type_b = entity_b->type();

			// 踩中地板
			if (type_a == EntityType::FLOOR or type_b == EntityType::FLOOR)
			{
				const auto handle_projectile_collision = [this]<typename T>(const FloorEntity* floor, T* entity) noexcept -> void //
							requires std::is_same_v<T, EnemyEntity> or std::is_same_v<T, PlayerEntity>
				{
					// todo: 应用地板的效果
					std::ignore = floor;
					std::ignore = entity;
				};

				if (type_a == EntityType::FLOOR)
				{
					std::println("接触地板: A: FLOOR, B: {}", type_b == EntityType::ENEMY ? "ENEMY" : "PLAYER");

					if (type_b == EntityType::ENEMY)
					{
						handle_projectile_collision(static_cast<const FloorEntity*>(entity_a), static_cast<EnemyEntity*>(entity_b));
					}
					else
					{
						PD_ASSERT(type_b == EntityType::PLAYER);
						handle_projectile_collision(static_cast<const FloorEntity*>(entity_a), static_cast<PlayerEntity*>(entity_b));
					}
				}
				else
				{
					std::println("接触地板: A: {}, B: FLOOR", type_a == EntityType::ENEMY ? "ENEMY" : "PLAYER");

					if (type_a == EntityType::ENEMY)
					{
						handle_projectile_collision(static_cast<const FloorEntity*>(entity_b), static_cast<EnemyEntity*>(entity_a));
					}
					else
					{
						PD_ASSERT(type_a == EntityType::PLAYER);
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

		void EndContact(b2Contact* contact) override
		{
			const auto* fixture_a = contact->GetFixtureA();
			const auto* fixture_b = contact->GetFixtureB();

			const auto* body_a = fixture_a->GetBody();
			const auto* body_b = fixture_b->GetBody();

			auto* entity_a = reinterpret_cast<Entity*>(body_a->GetUserData().pointer); // NOLINT(performance-no-int-to-ptr)
			auto* entity_b = reinterpret_cast<Entity*>(body_b->GetUserData().pointer); // NOLINT(performance-no-int-to-ptr)

			// todo: Entity::~Entity销毁body时也会调用该函数,此时再调用Entity的接口将会出问题
			// PD_ASSERT(entity_a != nullptr and entity_b != nullptr);
			if (entity_a == nullptr or entity_b == nullptr)
			{
				return;
			}

			const auto type_a = entity_a->type();
			const auto type_b = entity_b->type();

			// 离开地板
			if (type_a == EntityType::FLOOR or type_b == EntityType::FLOOR)
			{
				if (type_a == EntityType::FLOOR)
				{
					std::println("离开地板: A: FLOOR, B: {}", type_b == EntityType::ENEMY ? "ENEMY" : "PLAYER");
				}
				else
				{
					std::println("离开地板: A: {}, B: FLOOR", type_a == EntityType::ENEMY ? "ENEMY" : "PLAYER");
				}
			}
		}

		[[nodiscard]] auto inactive_entities() noexcept -> list_type&
		{
			return inactive_entities_;
		}
	};

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

		std::unique_ptr<b2World> physics_world_;

		PlayerEntity* player_;
		Movement movement_x_;
		Movement movement_y_;

		std::vector<std::unique_ptr<Entity>> entities_;

		ContactListener contact_listener_;

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

			auto top_wall = std::make_unique<WallEntity>(*physics_world_, top_position, top_size);
			auto bottom_wall = std::make_unique<WallEntity>(*physics_world_, bottom_position, bottom_size);
			auto left_wall = std::make_unique<WallEntity>(*physics_world_, left_position, left_size);
			auto right_wall = std::make_unique<WallEntity>(*physics_world_, right_position, right_size);

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
					auto entity = std::make_unique<FloorEntity>(*physics_world_, position, size, type);
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
				auto decoration = std::make_unique<DecorationEntity>(*physics_world_, position, size);
				entities_.emplace_back(std::move(decoration));
			}
		}

		auto create_player() noexcept -> void
		{
			auto player = std::make_unique<PlayerEntity>(
				*physics_world_,
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
			// 无重力世界
			physics_world_{std::make_unique<b2World>(b2Vec2{0, 0})},
			player_{nullptr},
			movement_x_{Movement::STAY},
			movement_y_{Movement::STAY}
		{
			// 设置碰撞监听器
			physics_world_->SetContactListener(&contact_listener_);

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
				else if (mbp->button == sf::Mouse::Button::Middle)
				{
					spawn_projectile_to<BounceProjectileEntity>(sf::Vector2f{mbp->position});
				}
				else if (mbp->button == sf::Mouse::Button::Right)
				{
					spawn_projectile_to<ChainProjectileEntity>(sf::Vector2f{mbp->position});
				}

				return;
			}

			//
		}

		auto update(const float delta) noexcept -> void
		{
			// 移除非活跃实体
			{
				auto& inactive_entities = contact_listener_.inactive_entities();

				auto r = std::ranges::remove_if(
					entities_,
					[&](const auto& entity) noexcept -> bool
					{
						return not entity->is_active() or std::ranges::contains(inactive_entities, entity.get());
					}
				);
				entities_.erase(r.begin(), entities_.end());

				inactive_entities.clear();
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
			physics_world_->Step(delta, 8, 3);

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
						*physics_world_,
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
						*physics_world_,
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
				else if constexpr (std::is_same_v<T, ChainProjectileEntity>)
				{
					// todo: 数量/半径/断裂力
					// 1大3小
					static std::array segment_radii{30.f, 15.f, 15.f, 15.f};
					static std::array break_force{150.f, 100.f, 100.f, 100.f};

					return std::make_unique<ChainProjectileEntity>(
						*physics_world_,
						EntityType::PLAYER,
						spawn_position,
						segment_radii,
						// todo: 飞弹速度
						direction_normalized * 400.f,
						break_force,
						// 存在5秒
						5.f
					);
				}
				else
				{
					PD_COMPILER_UNREACHABLE();
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

	inline auto ChainProjectileEntity::split_on_broken() noexcept -> void
	{
		std::println("因关节[{}]断裂而解体", broken_joint_index_);

		// 创建分散的小子弹
		for (std::size_t i = 0; i < segments_.size(); ++i)
		{
			const auto& segment = segments_[i];

			const auto physics_position = segment->GetPosition();
			const auto physics_velocity = segment->GetLinearVelocity();

			const auto position = pd::Constant::from_physics(physics_position);
			const auto velocity = pd::Constant::from_physics(physics_velocity);

			std::println(
				"子飞弹({})> Position: ({:1f},{:1f}), Velocity: ({:1f},{:1f})[{}]",
				i,
				position.x,
				position.y,
				velocity.x,
				velocity.y,
				velocity.length()
			);

			Playground::playground->spawn_projectile_toward<BaseProjectileEntity>(position, velocity);
		}

		// 标记当前飞弹不再活跃
		active_ = false;
	}
}
