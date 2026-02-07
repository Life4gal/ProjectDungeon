// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <algorithm>
#include <print>

#include <box2d/box2d.h>

#include <SFML/Graphics.hpp>

#include <imgui.h>

#include <pd/constants.hpp>
#include <pd/utility/enumeration.hpp>

namespace playground
{
	enum class EntityType : std::uint8_t
	{
		// 墙
		WALL = 0b0000'0001,
		// 地形/装饰物
		DECORATION = 0b0000'0010,

		// 敌人实体
		ENEMY = 0b0000'0100,
		// 玩家实体
		PLAYER = 0b0000'1000,

		// 飞弹
		PROJECTILE = 0b0001'0000,

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

	class PhysicsDesc final // NOLINT(clang-diagnostic-padded)
	{
	public:
		b2BodyType type;

		// 是否可旋转
		bool can_rotate;
		// 开启连续碰撞检测
		bool is_bullet;
		// 阻尼(平滑移动)
		float linear_damping;
		// 初始(像素)位置
		b2Vec2 initial_position;
		// (像素)初速度
		b2Vec2 initial_velocity;
		// (像素)大小
		b2Vec2 size;

		// 形状
		b2Shape* shape;
		// 密度
		float density;
		// 摩擦力
		float friction;
		// 弹性
		float restitution;

		// 碰撞过滤类型
		EntityType category;
		// 碰撞遮罩
		EntityType category_mask;
	};

	class Entity
	{
	protected:
		b2Body* body_;

		// box2d --> 米
		// SFML --> 像素
		sf::RectangleShape shape_;

		bool active_;
		RenderLayer render_layer_;

	private:
		auto create_physics(b2World& world, const PhysicsDesc& desc) noexcept -> void
		{
			// 创建BODY
			{
				b2BodyDef body_def{};
				body_def.type = desc.type;
				body_def.position = pd::Constant::to_physics(desc.initial_position);
				body_def.linearVelocity = pd::Constant::to_physics(desc.initial_velocity);
				body_def.fixedRotation = not desc.can_rotate;
				body_def.bullet = desc.is_bullet;
				body_def.linearDamping = desc.linear_damping;

				body_ = world.CreateBody(&body_def);
			}
			// 创建FIXTURE
			{
				b2FixtureDef fixture_def{};
				fixture_def.shape = desc.shape;
				fixture_def.density = desc.density;
				fixture_def.friction = desc.friction;
				fixture_def.restitution = desc.restitution;

				fixture_def.filter.categoryBits = static_cast<uint16>(desc.category);
				fixture_def.filter.maskBits = static_cast<uint16>(desc.category_mask);

				body_->CreateFixture(&fixture_def);
				body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
			}
		}

	public:
		Entity(b2World& world, const RenderLayer render_layer, const PhysicsDesc& desc) noexcept
			: body_{nullptr},
			  shape_{{desc.size.x, desc.size.y}},
			  active_{true},
			  render_layer_{render_layer}
		{
			// 中心对齐
			shape_.setOrigin(shape_.getSize() / 2.f);

			create_physics(world, desc);
		}

		Entity(const Entity&) noexcept = delete;
		auto operator=(const Entity&) noexcept -> Entity& = delete;

		Entity(Entity&&) noexcept = default;
		auto operator=(Entity&&) noexcept -> Entity& = default;

		virtual ~Entity() noexcept
		{
			auto* world = body_->GetWorld();
			world->DestroyBody(body_);
		}

		virtual auto type() const noexcept -> EntityType = 0;

		virtual auto update(const float delta) noexcept -> void
		{
			// 同步box2d世界的位置
			const auto position = body_->GetPosition();

			shape_.setPosition(pd::Constant::from_physics(position));
		}

		virtual auto render(sf::RenderWindow& window) noexcept -> void
		{
			if (not active_)
			{
				return;
			}

			const auto radians = body_->GetAngle();
			shape_.setRotation(sf::radians(radians));

			window.draw(shape_);
		}

		[[nodiscard]] auto is_active() const noexcept -> bool
		{
			return active_;
		}

		[[nodiscard]] auto get_position() const noexcept -> sf::Vector2f
		{
			return shape_.getPosition();
		}

		[[nodiscard]] auto get_velocity() const noexcept -> sf::Vector2f
		{
			const auto v = body_->GetLinearVelocity();
			return pd::Constant::from_physics(v);
		}

		[[nodiscard]] auto get_render_layer() const noexcept -> RenderLayer
		{
			return render_layer_;
		}
	};

	// 玩家
	class PlayerEntity : public Entity
	{
	public:
		//

	private:
		[[nodiscard]] static auto make_desc(const sf::Vector2f start_position, const sf::Vector2f size) noexcept -> PhysicsDesc
		{
			static auto shape = [bs = 0.5f * pd::Constant::to_physics(size)]-> b2PolygonShape
			{
				b2PolygonShape s{};
				s.SetAsBox(bs.x, bs.y);
				return s;
			}();

			return
			{
					.type = b2_dynamicBody,
					.can_rotate = false,
					.is_bullet = false,
					.linear_damping = 1,
					.initial_position = {start_position.x, start_position.y},
					.initial_velocity = {0, 0},
					.size = {size.x, size.y},
					.shape = &shape,
					.density = 1,
					.friction = 0.3f,
					.restitution = 0.05f,
					.category = EntityType::PLAYER,
					.category_mask = EntityType::WALL | EntityType::DECORATION,
			};
		}

		float move_speed_;

	public:
		PlayerEntity(b2World& world, const sf::Vector2f start_position, const sf::Vector2f size, const float move_speed) noexcept
			: Entity{world, RenderLayer::PLAYER, make_desc(start_position, size)},
			  move_speed_{move_speed}
		{
			// 玩家颜色
			shape_.setFillColor(sf::Color::Blue);
		}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::PLAYER;
		}

		auto apply_force(b2Vec2 force) const noexcept -> void
		{
			if (force.LengthSquared() > 0)
			{
				force.Normalize();
			}

			force *= body_->GetMass() * move_speed_;
			body_->ApplyForceToCenter(force, true);
		}
	};

	// 墙壁
	class WallEntity : public Entity
	{
	public:
		//

	private:
		[[nodiscard]] static auto make_desc(const sf::Vector2f position, const sf::Vector2f size) noexcept -> PhysicsDesc
		{
			static auto shape = [bs = 0.5f * pd::Constant::to_physics(size)]-> b2PolygonShape
			{
				b2PolygonShape s{};
				s.SetAsBox(bs.x, bs.y);
				return s;
			}();

			return
			{
					.type = b2_staticBody,
					.can_rotate = false,
					.is_bullet = false,
					.linear_damping = 5.f,
					.initial_position = {position.x, position.y},
					.initial_velocity = {0, 0},
					.size = {size.x, size.y},
					.shape = &shape,
					.density = 0,
					.friction = 0.5f,
					.restitution = 0.7f,
					.category = EntityType::WALL,
					.category_mask = EntityType::ENEMY | EntityType::PLAYER | EntityType::PROJECTILE,
			};
		}

	public:
		WallEntity(b2World& world, const sf::Vector2f position, const sf::Vector2f size) noexcept
			: Entity{world, RenderLayer::WALL, make_desc(position, size)}
		{
			// 墙颜色
			shape_.setFillColor(sf::Color::Yellow);
		}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::WALL;
		}
	};

	// 地形/装饰物
	class DecorationEntity : public Entity
	{
	public:
		//

	private:
		[[nodiscard]] static auto make_desc(const sf::Vector2f position, const sf::Vector2f size) noexcept -> PhysicsDesc
		{
			static auto shape = [bs = 0.5f * pd::Constant::to_physics(size)] -> b2PolygonShape
			{
				b2PolygonShape s{};
				s.SetAsBox(bs.x, bs.y);
				return s;
			}();

			return
			{
					.type = b2_staticBody,
					.can_rotate = false,
					.is_bullet = false,
					.linear_damping = 5.f,
					.initial_position = {position.x, position.y},
					.initial_velocity = {0, 0},
					.size = {size.x, size.y},
					.shape = &shape,
					.density = 0,
					.friction = 0.35f,
					.restitution = 0.4f,
					.category = EntityType::DECORATION,
					.category_mask = EntityType::ENEMY | EntityType::PLAYER | EntityType::PROJECTILE,
			};
		}

	public:
		DecorationEntity(b2World& world, const sf::Vector2f position, const sf::Vector2f size) noexcept
			: Entity{world, RenderLayer::DECORATION, make_desc(position, size)}
		{
			// 地形/装饰物颜色
			shape_.setFillColor(sf::Color::Yellow);
		}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::DECORATION;
		}
	};

	// 飞弹
	class ProjectileEntity : public Entity
	{
	public:
		using size_type = std::uint32_t;

	private:
		// 飞行时长(生命周期)
		float lifetime_;
		float lifetime_current_;

		// 弹跳次数(撞击墙壁/装饰物)
		size_type bounces_;
		size_type bounces_current_;

		// 下次碰撞时销毁
		bool destroy_on_next_collision_;

		// 拖尾效果
		sf::ConvexShape tail_;

		[[nodiscard]] static auto make_desc(const sf::Vector2f position, const float radius, sf::Vector2f velocity) noexcept -> PhysicsDesc
		{
			static auto shape = [r = 0.5f * pd::Constant::to_physics(radius)] -> b2CircleShape
			{
				b2CircleShape s{};
				s.m_radius = r;
				return s;
			}();

			return
			{
					.type = b2_dynamicBody,
					.can_rotate = true,
					.is_bullet = true,
					.linear_damping = 0.05f,
					.initial_position = {position.x, position.y},
					.initial_velocity = {velocity.x, velocity.y},
					.size = {radius, radius},
					.shape = &shape,
					.density = 0.5f,
					.friction = 0,
					.restitution = 0.8f,
					.category = EntityType::PROJECTILE,
					.category_mask = EntityType::ENEMY | EntityType::PLAYER | EntityType::WALL | EntityType::DECORATION,
			};
		}

	public:
		ProjectileEntity(
			b2World& world,
			const sf::Vector2f position,
			const float radius,
			const sf::Vector2f velocity,
			const float lifetime,
			const size_type bounces
		) noexcept
			: Entity{world, RenderLayer::PROJECTILE, make_desc(position, radius, velocity)},
			  lifetime_{lifetime},
			  lifetime_current_{0},
			  bounces_{bounces},
			  bounces_current_{0},
			  destroy_on_next_collision_{false},
			  // 三角形拖尾
			  tail_{3}
		{
			// 飞弹颜色
			shape_.setFillColor(sf::Color::Red);
			// 拖尾颜色
			tail_.setFillColor({255, 100, 100, 150});
		}

		auto type() const noexcept -> EntityType override
		{
			return EntityType::PROJECTILE;
		}

		auto update(const float delta) noexcept -> void override
		{
			Entity::update(delta);

			lifetime_current_ += delta;
			if (lifetime_current_ >= lifetime_)
			{
				active_ = false;
			}

			// 如果标记为下次碰撞销毁,且速度很慢,销毁
			if (destroy_on_next_collision_)
			{
				if (const auto velocity = body_->GetLinearVelocity();
					velocity.LengthSquared() < 1)
				{
					active_ = false;
				}
			}
		}

		auto render(sf::RenderWindow& window) noexcept -> void override
		{
			Entity::render(window);

			// 添加拖尾效果

			const auto position = shape_.getPosition();

			auto velocity = body_->GetLinearVelocity();
			velocity.Normalize();

			const auto direction = sf::Vector2f{velocity.x, velocity.y};
			const auto left_perpendicular = sf::Vector2f{-direction.y, direction.x};

			tail_.setPoint(0, position + direction * 5.f);
			tail_.setPoint(1, position - direction * 10.f + left_perpendicular * 5.f);
			tail_.setPoint(2, position - direction * 10.f - left_perpendicular * 5.f);

			window.draw(tail_);
		}

		// 处理反弹
		auto on_bounce() noexcept -> void
		{
			bounces_current_ += 1;
			if (bounces_current_ >= bounces_)
			{
				// 达到最大反弹次数后,标记为下次碰撞时销毁
				destroy_on_next_collision_ = true;

				// 减少弹性, 让飞弹逐渐停止
				for (auto* f = body_->GetFixtureList(); f; f = f->GetNext())
				{
					f->SetRestitution(0.3f);
				}
			}

			// 每次反弹后稍微减少速度
			const auto velocity = body_->GetLinearVelocity();
			// todo: 减速系数
			body_->SetLinearVelocity(0.85f * velocity);
		}
	};

	// 碰撞监听器
	class ContactListener : public b2ContactListener
	{
	public:
		using list_type = std::vector<Entity*>;

	private:
		list_type inactive_entities_;

	public:
		ContactListener() noexcept
			: b2ContactListener{} {}

		void BeginContact(b2Contact* contact) override
		{
			const auto* fixture_a = contact->GetFixtureA();
			const auto* fixture_b = contact->GetFixtureB();

			const auto* body_a = fixture_a->GetBody();
			const auto* body_b = fixture_b->GetBody();

			auto* entity_a = reinterpret_cast<Entity*>(body_a->GetUserData().pointer); // NOLINT(performance-no-int-to-ptr)
			auto* entity_b = reinterpret_cast<Entity*>(body_b->GetUserData().pointer); // NOLINT(performance-no-int-to-ptr)

			const auto type_a = entity_a->type();
			const auto type_b = entity_b->type();

			// 飞弹碰撞
			if (type_a == EntityType::PROJECTILE or type_b == EntityType::PROJECTILE)
			{
				const auto handle_projectile_collision = [this](ProjectileEntity* projectile, const Entity* entity) noexcept -> void
				{
					// 碰撞特效/音效

					if (const auto type = entity->type();
						type == EntityType::WALL or type == EntityType::DECORATION)
					{
						// 触发反弹
						projectile->on_bounce();

						std::println("命中WALL/DECORATION");
					}
					else if (type == EntityType::ENEMY)
					{
						std::println("命中ENEMY");
						inactive_entities_.push_back(projectile);
					}
					else if (type == EntityType::PLAYER)
					{
						std::println("命中Player");
						inactive_entities_.push_back(projectile);
					}
				};

				if (type_a == EntityType::PROJECTILE)
				{
					handle_projectile_collision(static_cast<ProjectileEntity*>(entity_a), entity_b); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
				}
				else
				{
					handle_projectile_collision(static_cast<ProjectileEntity*>(entity_b), entity_a); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
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
		//

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

		auto create_decoration() noexcept -> void
		{
			constexpr std::array positions
			{
					sf::Vector2f{400, 300},
					sf::Vector2f{800, 500},
					sf::Vector2f{1200, 400},
					sf::Vector2f{600, 700},
					sf::Vector2f{1200, 600},
					sf::Vector2f{1600, 900},
			};

			for (const auto position: positions)
			{
				// todo: 障碍物大小
				auto decoration = std::make_unique<DecorationEntity>(*physics_world_, position, sf::Vector2f{50, 50});
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
				sf::Vector2f{40, 40},
				// todo: 玩家速度
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
					const auto player_position = player_->get_position();
					const auto target_position = sf::Vector2f{mbp->position};

					const auto direction = target_position - player_position;
					const auto direction_normalized = direction.normalized();
					const auto spawn_position = player_position + direction_normalized * 30.f;

					auto projectile = std::make_unique<ProjectileEntity>(
						*physics_world_,
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
					entities_.emplace_back(std::move(projectile));
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
			std::ranges::sort(entities_, std::ranges::less{}, &Entity::get_render_layer);

			// 渲染所有实体
			for (const auto& entity: entities_)
			{
				entity->render(window);
			}
		}

		auto show_info() noexcept -> void
		{
			ImGui::Begin("物理调试信息");

			// 显示实体数量
			ImGui::Text("实体数量: %zu", entities_.size());

			// 显示玩家位置/速度
			{
				const auto position = player_->get_position();
				const auto velocity = player_->get_velocity();

				ImGui::Text("玩家位置: (%.1f, %.1f), 玩家速度: (%.1f, %.1f)", position.x, position.y, velocity.x, velocity.y);
			}

			// 显示飞弹信息
			{
				const auto count = std::ranges::count(entities_, EntityType::PROJECTILE, &Entity::type);
				ImGui::Text("活动飞弹: %td", count);
			}

			ImGui::End();
		}
	};
}
