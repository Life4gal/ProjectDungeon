// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/game.hpp>

#include <menu/pause.hpp>

// =========
// 管理器

#include <manager/resource.hpp>
#include <manager/audio_player.hpp>
#include <manager/event.hpp>
#include <manager/random.hpp>

// =========
// 物理世界特别处理

#include <utility/physics.hpp>
#include <component/physics_body.hpp>
#include <box2d/box2d.h>

// =========
// 测试用

#include <component/player_controller.hpp>
#include <component/state.hpp>
#include <component/enemy.hpp>

#include <designer/level.hpp>
#include <designer/player.hpp>
#include <designer/projectile.hpp>
#include <factory/level.hpp>
#include <factory/player.hpp>
#include <factory/projectile.hpp>

// =========
// 监听

#include <listener/camera.hpp>
#include <listener/actor.hpp>
// #include <listener/enemy.hpp>
#include <listener/player.hpp>
#include <listener/projectile.hpp>
#include <listener/door.hpp>
#include <listener/room.hpp>

// =========
// 更新

#include <update/graveyard.hpp>
#include <update/player_controller.hpp>
#include <update/ai.hpp>
#include <update/physics_world.hpp>
#include <update/sync_physics_transform.hpp>
#include <update/process_physics_events.hpp>
#include <update/actor.hpp>
#include <update/projectile.hpp>
#include <update/sprite_animation.hpp>

// =========
// 渲染

#include <render/camera.hpp>
#include <render/floor.hpp>
#include <render/wall.hpp>
#include <render/door.hpp>
#include <render/enemy.hpp>
#include <render/player.hpp>
#include <render/health_mana_bar.hpp>
#include <render/projectile.hpp>

// =========
// 依赖

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>

namespace pd::scene
{
	namespace
	{
		// 游戏音乐
		// TODO: 多个音乐
		// TODO: 暂停菜单是否需要切换音乐?
		constexpr std::string_view GameMusic = R"(.\media\musics\game.wav)";

		auto on_destroy_physics_body(entt::registry& registry, const entt::entity entity) noexcept -> void
		{
			const auto [body_id] = registry.get<const component::physics_body::Id>(entity);
			b2DestroyBody(body_id);
		}

		// 创建物理世界
		auto create_physics_world(entt::registry& registry) noexcept -> void
		{
			auto& world_id = utility::Physics::world_id;

			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NULL(world_id), "重复创建物理世界");

			auto def = b2DefaultWorldDef();
			// 无重力世界(俯视角)
			def.gravity = b2Vec2_zero;
			world_id = b2CreateWorld(&def);

			// 订阅组件销毁事件,以便在组件销毁时销毁物理刚体
			// 如此便不需要在销毁实体前手动调用deattach函数销毁物理刚体组件
			registry.on_destroy<component::physics_body::Id>().connect<&on_destroy_physics_body>();
		}

		// 销毁物理世界
		auto destroy_physics_world(entt::registry& registry) noexcept -> void
		{
			auto& world_id = utility::Physics::world_id;

			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(world_id), "物理世界未创建");

			registry.on_destroy<component::physics_body::Id>().disconnect<&on_destroy_physics_body>();

			b2DestroyWorld(world_id);
			world_id = b2_nullWorldId;
		}

		// 绘制物理世界
		constexpr auto rgb_to_rgba = [](const b2HexColor color) noexcept -> sf::Color
		{
			return sf::Color{(static_cast<std::uint32_t>(color) << 8) | 0xff};
		};
		auto g_physics_world_draw = [] noexcept -> b2DebugDraw
		{
			using utility::Physics;

			auto draw = b2DefaultDebugDraw();

			draw.DrawPolygonFcn = [](const b2Vec2* vertices, const int vertex_count, const b2HexColor color, void* context) noexcept -> void
			{
				const auto sf_color = rgb_to_rgba(color);
				auto* window = static_cast<sf::RenderWindow*>(context);

				sf::ConvexShape polygon{};

				polygon.setPointCount(vertex_count);
				for (int i = 0; i < vertex_count; ++i)
				{
					const auto& vertex = vertices[i];

					const auto pixels_position = Physics::from_physics(vertex);
					polygon.setPoint(i, pixels_position);
				}

				polygon.setFillColor(sf::Color::Transparent);
				polygon.setOutlineColor(sf_color);
				polygon.setOutlineThickness(2);

				window->draw(polygon);
			};
			draw.DrawSolidPolygonFcn = [](
				const b2Transform transform,
				const b2Vec2* vertices,
				const int vertex_count,
				[[maybe_unused]] const float radius,
				const b2HexColor color,
				void* context
			) noexcept -> void
					{
						const auto sf_color = rgb_to_rgba(color);
						const auto sf_color_half_transparent = sf::Color{sf_color.r, sf_color.g, sf_color.b, 128};
						auto* window = static_cast<sf::RenderWindow*>(context);

						sf::ConvexShape polygon{};

						const b2Vec2 origin = transform.p;
						polygon.setPointCount(vertex_count);
						for (int i = 0; i < vertex_count; ++i)
						{
							const auto& vertex = vertices[i];
							const auto rotated = b2RotateVector(transform.q, vertex);
							const auto position = origin + rotated;

							const auto pixels_position = Physics::from_physics(position);
							polygon.setPoint(i, pixels_position);
						}

						polygon.setFillColor(sf_color_half_transparent);
						polygon.setOutlineColor(sf_color);
						polygon.setOutlineThickness(1);

						window->draw(polygon);
					};
			draw.DrawCircleFcn = [](const b2Vec2 center, const float radius, const b2HexColor color, void* context) noexcept -> void
			{
				const auto pixels_radius = Physics::from_physics(radius);
				const auto pixels_center = Physics::from_physics(center);
				const auto sf_color = rgb_to_rgba(color);
				auto* window = static_cast<sf::RenderWindow*>(context);

				sf::CircleShape circle{};

				circle.setRadius(pixels_radius);
				circle.setOrigin({pixels_radius, pixels_radius});
				circle.setPosition(pixels_center);

				circle.setFillColor(sf::Color::Transparent);
				circle.setOutlineColor(sf_color);
				circle.setOutlineThickness(2);

				window->draw(circle);
			};
			draw.DrawSolidCircleFcn = [](const b2Transform transform, const float radius, const b2HexColor color, void* context) noexcept -> void
			{
				const auto pixels_radius = Physics::from_physics(radius);
				const auto pixels_center = Physics::from_physics(transform.p);
				const auto sf_color = rgb_to_rgba(color);
				const auto sf_color_half_transparent = sf::Color{sf_color.r, sf_color.g, sf_color.b, 128};
				auto* window = static_cast<sf::RenderWindow*>(context);

				sf::CircleShape circle{};

				circle.setRadius(pixels_radius);
				circle.setOrigin({pixels_radius, pixels_radius});
				circle.setPosition(pixels_center);

				circle.setFillColor(sf_color_half_transparent);
				circle.setOutlineColor(sf_color);
				circle.setOutlineThickness(1);

				window->draw(circle);
			};
			draw.DrawSolidCapsuleFcn = [](const b2Vec2 p1, const b2Vec2 p2, const float radius, const b2HexColor color, void* context) noexcept -> void
			{
				const auto pixels_p1 = Physics::from_physics(p1);
				const auto pixels_p2 = Physics::from_physics(p2);
				const auto pixels_radius = Physics::from_physics(radius);
				const auto sf_color = rgb_to_rgba(color);
				const auto sf_color_half_transparent = sf::Color{sf_color.r, sf_color.g, sf_color.b, 128};
				auto* window = static_cast<sf::RenderWindow*>(context);

				const auto direction = pixels_p2 - pixels_p1;
				const auto length = direction.length();
				const auto angle = std::atan2(direction.y, direction.x);

				sf::RectangleShape rect{};
				rect.setSize({length, pixels_radius * 2});
				rect.setOrigin({0, pixels_radius});
				rect.setPosition(pixels_p1);
				rect.setRotation(sf::radians(angle));

				sf::CircleShape end1{};
				end1.setRadius(pixels_radius);
				end1.setOrigin({pixels_radius, pixels_radius});
				end1.setPosition(pixels_p1);

				sf::CircleShape end2{};
				end2.setRadius(pixels_radius);
				end2.setOrigin({pixels_radius, pixels_radius});
				end2.setPosition(pixels_p2);

				rect.setFillColor(sf_color_half_transparent);
				end1.setFillColor(sf_color_half_transparent);
				end2.setFillColor(sf_color_half_transparent);

				window->draw(rect);
				window->draw(end1);
				window->draw(end2);
			};
			draw.DrawSegmentFcn = [](const b2Vec2 p1, const b2Vec2 p2, const b2HexColor color, void* context) noexcept -> void
			{
				const auto pixels_p1 = Physics::from_physics(p1);
				const auto pixels_p2 = Physics::from_physics(p2);
				const auto sf_color = rgb_to_rgba(color);
				auto* window = static_cast<sf::RenderWindow*>(context);

				const std::array<sf::Vertex, 2> line
				{{
						{.position = pixels_p1, .color = sf_color, .texCoords = {}},
						{.position = pixels_p2, .color = sf_color, .texCoords = {}},
				}};

				window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
			};
			// draw.DrawTransformFcn = nullptr;
			// draw.DrawPointFcn = nullptr;
			// draw.DrawStringFcn = nullptr;

			draw.drawShapes = true;
			draw.drawJoints = true;
			draw.drawBounds = true;
			draw.drawMass = true;
			draw.drawContacts = true;
			draw.drawContactNormals = true;

			return draw;
		}();
		auto g_physics_world_draw_on = false;

		auto g_stop_ai = false;
	}

	auto Game::start_game() noexcept -> bool
	{
		// 进入地下城

		manager::Random::seed(123456);

		// 关卡
		const auto level = designer::Level::generate(8, 5, 15, 4, 2);
		factory::Level::create(registry_, level);
		manager::Event::enqueue(event::room::Enter{});

		// 玩家
		const auto& start_room = level.rooms.at(level.start_position);
		auto player = designer::Player::test_character();
		player.transform.x += start_room.offset_x;
		player.transform.y += start_room.offset_y;
		const auto player_entity = factory::Player::spawn(registry_, player);
		registry_.ctx().emplace<component::player_controller::Target>(player_entity);

		// 相机
		manager::Event::enqueue(event::camera::Set{.x = start_room.offset_x, .y = start_room.offset_y, .width = 1080, .height = 720});

		return true;
	}

	auto Game::load_game() noexcept -> bool
	{
		return false;
	}

	auto Game::restart_game() noexcept -> void
	{
		//
	}

	Game::Game() noexcept
		: music_{manager::InvalidHandler},
		  frame_delta_{sf::seconds(1)},
		  total_elapsed_{sf::Time::Zero},
		  play_elapsed_{sf::Time::Zero},
		  is_paused_{false} {}

	Game::Game(Game&&) noexcept = default;

	auto Game::operator=(Game&&) noexcept -> Game& = default;

	Game::~Game() noexcept = default;

	auto Game::on_loaded() noexcept -> void
	{
		// 也许菜单创建可以延迟到第一次打开菜单时?不过目前先在这里创建好了
		pause_ = std::make_unique<menu::Pause>(is_paused_);

		music_ = manager::Music::load(GameMusic);

		// 物理世界
		create_physics_world(registry_);

		// 相机
		manager::Event::subscribe<event::camera::Set, &listener::camera::on_set>(registry_);
		manager::Event::subscribe<event::camera::MoveTo, &listener::camera::on_move_to>(registry_);
		manager::Event::subscribe<event::camera::Translate, &listener::camera::on_translate>(registry_);
		manager::Event::subscribe<event::camera::Resize, &listener::camera::on_resize>(registry_);
		// Actor
		manager::Event::subscribe<event::actor::Hurt, &listener::actor::on_hurt>(registry_);
		manager::Event::subscribe<event::actor::Dead, &listener::actor::on_dead>(registry_);
		// 敌人
		//
		// 玩家
		manager::Event::subscribe<event::player::MoveTo, &listener::player::on_move_to>(registry_);
		manager::Event::subscribe<event::player::Translate, &listener::player::on_translate>(registry_);
		// 飞弹
		manager::Event::subscribe<event::physics::ContactBegin, &listener::projectile::on_contact_begin>(registry_);
		manager::Event::subscribe<event::physics::ContactEnd, &listener::projectile::on_contact_end>(registry_);
		// 地下城 -- 关卡 -- 房间-- 门
		manager::Event::subscribe<event::physics::ContactBegin, &listener::door::on_contact_begin>(registry_);
		manager::Event::subscribe<event::physics::ContactEnd, &listener::door::on_contact_end>(registry_);
		manager::Event::subscribe<event::physics::SensorBegin, &listener::door::on_sensor_begin>(registry_);
		manager::Event::subscribe<event::physics::SensorEnd, &listener::door::on_sensor_end>(registry_);
		manager::Event::subscribe<event::door::RequestOpen, &listener::door::on_request_open>(registry_);
		manager::Event::subscribe<event::door::RequestClose, &listener::door::on_request_close>(registry_);
		// 地下城 -- 关卡-- 房间
		manager::Event::subscribe<event::actor::Dead, &listener::room::on_dead>(registry_);
		manager::Event::subscribe<event::room::Leave, &listener::room::on_leave>(registry_);
		manager::Event::subscribe<event::room::Enter, &listener::room::on_enter>(registry_);

		// 
	}

	auto Game::on_initialized() noexcept -> void
	{
		manager::AudioPlayer::play(music_);

		// 这里可以检查是否存在存档?
		start_game();
	}

	auto Game::on_unloaded() noexcept -> void
	{
		manager::AudioPlayer::stop(music_);
		music_ = manager::InvalidHandler;

		pause_.reset();

		// 销毁所有实体(如果有)
		factory::Level::destroy(registry_);
		factory::Player::destroy_all(registry_);

		// 相机
		manager::Event::unsubscribe<event::camera::Set, &listener::camera::on_set>(registry_);
		manager::Event::unsubscribe<event::camera::MoveTo, &listener::camera::on_move_to>(registry_);
		manager::Event::unsubscribe<event::camera::Translate, &listener::camera::on_translate>(registry_);
		manager::Event::unsubscribe<event::camera::Resize, &listener::camera::on_resize>(registry_);
		// Actor
		manager::Event::unsubscribe<event::actor::Hurt, &listener::actor::on_hurt>(registry_);
		manager::Event::unsubscribe<event::actor::Dead, &listener::actor::on_dead>(registry_);
		// 敌人
		//
		// 玩家
		manager::Event::unsubscribe<event::player::MoveTo, &listener::player::on_move_to>(registry_);
		manager::Event::unsubscribe<event::player::Translate, &listener::player::on_translate>(registry_);
		// 飞弹
		manager::Event::unsubscribe<event::physics::ContactBegin, &listener::projectile::on_contact_begin>(registry_);
		manager::Event::unsubscribe<event::physics::ContactEnd, &listener::projectile::on_contact_end>(registry_);
		// 地下城 -- 关卡-- 房间-- 门
		manager::Event::unsubscribe<event::physics::ContactBegin, &listener::door::on_contact_begin>(registry_);
		manager::Event::unsubscribe<event::physics::ContactEnd, &listener::door::on_contact_end>(registry_);
		manager::Event::unsubscribe<event::physics::SensorBegin, &listener::door::on_sensor_begin>(registry_);
		manager::Event::unsubscribe<event::physics::SensorEnd, &listener::door::on_sensor_end>(registry_);
		manager::Event::unsubscribe<event::door::RequestOpen, &listener::door::on_request_open>(registry_);
		manager::Event::unsubscribe<event::door::RequestClose, &listener::door::on_request_close>(registry_);
		// 地下城 -- 关卡-- 房间
		manager::Event::unsubscribe<event::actor::Dead, &listener::room::on_dead>(registry_);
		manager::Event::unsubscribe<event::room::Leave, &listener::room::on_leave>(registry_);
		manager::Event::unsubscribe<event::room::Enter, &listener::room::on_enter>(registry_);

		// 最后销毁物理世界
		destroy_physics_world(registry_);
	}

	auto Game::handle_event(const sf::Event& event) noexcept -> void
	{
		if (const auto& io = ImGui::GetIO();
			io.WantCaptureKeyboard || io.WantCaptureMouse)
		{
			return;
		}

		if (is_paused_)
		{
			pause_->handle_event(event);
		}
		else
		{
			if (const auto* kp = event.getIf<sf::Event::KeyPressed>())
			{
				using sf::Keyboard::Key;
				namespace player_controller = component::player_controller;

				if (kp->code == Key::Escape)
				{
					is_paused_ = true;
				}
				// =====================
				// PHYSICS_WORLD
				// =====================

				else if (kp->code == Key::Num1)
				{
					g_physics_world_draw_on = not g_physics_world_draw_on;
				}
				// =====================
				// PLAYER_CONTROLLER
				// =====================
				else if (kp->code == Key::A)
				{
					registry_.ctx().emplace<player_controller::HorizontalMovement>(player_controller::MovementType::BACKWARD);
				}
				else if (kp->code == Key::D)
				{
					registry_.ctx().emplace<player_controller::HorizontalMovement>(player_controller::MovementType::FORWARD);
				}
				else if (kp->code == Key::W)
				{
					registry_.ctx().emplace<player_controller::VerticalMovement>(player_controller::MovementType::BACKWARD);
				}
				else if (kp->code == Key::S)
				{
					registry_.ctx().emplace<player_controller::VerticalMovement>(player_controller::MovementType::FORWARD);
				}
				else if (kp->code == Key::Left or kp->code == Key::Right or kp->code == Key::Up or kp->code == Key::Down)
				{
					// TODO: 蓝图持久化?
					const static auto projectile_blueprint = designer::Projectile::standard();

					if (const auto* target = registry_.ctx().find<const player_controller::Target>())
					{
						const auto direction = [&] noexcept -> sf::Vector2f
						{
							if (kp->code == Key::Left)
							{
								return {-1, 0};
							}
							if (kp->code == Key::Right)
							{
								return {1, 0};
							}
							if (kp->code == Key::Up)
							{
								return {0, -1};
							}
							return {0, 1};
						}();

						factory::Projectile::spawn(registry_, projectile_blueprint, target->entity, direction);
					}
				}
				else if (kp->code == Key::Q)
				{
					g_stop_ai = not g_stop_ai;
				}
				else if (kp->code == Key::E)
				{
					using namespace component;

					if (kp->control)
					{
						// 击杀所有敌人

						for (const auto view = registry_.view<state::InCameraArea, tags::Enemy>();
						     const auto [entity]: view.each())
						{
							manager::Event::enqueue(event::actor::Dead{.attacker = entt::null, .victim = entity});
						}
					}
					else
					{
						// 所有敌人生命值减半
						for (const auto view = registry_.view<state::InCameraArea, tags::Enemy, actor::Health>();
						     const auto [entity, health]: view.each())
						{
							manager::Event::enqueue(event::actor::Hurt{.attacker = entt::null, .victim = entity, .damage = health.health / 2.f});
						}
					}
				}
				// =====================
				// CAMERA
				// =====================
				else if (kp->code == Key::Z)
				{
					manager::Event::enqueue(event::camera::Resize{.width = 1080, .height = 720});
				}
				else if (kp->code == Key::X)
				{
					manager::Event::enqueue(event::camera::Resize{.width = 540, .height = 360});
				}
				else if (kp->code == Key::C)
				{
					manager::Event::enqueue(event::camera::Resize{.width = 270, .height = 180});
				}
				// =====================
				// DUNGEON -- LEVEL -- ROOM -- DOOR
				// =====================
				else if (kp->code == Key::O)
				{
					manager::Event::enqueue(event::door::RequestOpen{});
				}
				else if (kp->code == Key::P)
				{
					manager::Event::enqueue(event::door::RequestClose{});
				}
			}
			else if (const auto* kr = event.getIf<sf::Event::KeyReleased>())
			{
				using sf::Keyboard::Key;
				namespace player_controller = component::player_controller;

				// =====================
				// PLAYER_CONTROLLER
				// =====================
				if (kr->code == Key::A or kr->code == Key::D)
				{
					registry_.ctx().erase<player_controller::HorizontalMovement>();
				}
				else if (kr->code == Key::W or kr->code == Key::S)
				{
					registry_.ctx().erase<player_controller::VerticalMovement>();
				}
			}
		}
	}

	auto Game::update(const sf::Time delta) noexcept -> void
	{
		frame_delta_ = delta;
		total_elapsed_ += delta;
		if (not is_paused_)
		{
			play_elapsed_ += delta;
		}

		if (is_paused_)
		{
			pause_->update(delta);
		}
		else
		{
			// 我们让(且只让)墓地来销毁实体
			// 我们让其在每一帧开头(或者说每一帧末尾)来销毁那些被标记死亡的实体
			// 如此便能够保证那些压入事件中的实体在被监听器处理时依然有效
			// TODO: 这会引入一个新的问题,被标记死亡的实体并没有被立刻销毁,如果其存在物理刚体,那这一帧内该物理刚体依然会起效果(且该效果并非我们本意)
			update::graveyard(registry_, delta);

			update::player_controller(registry_, delta);

			if (not g_stop_ai)
			{
				update::ai(registry_, delta);
			}

			update::physics_world(registry_, delta);
			update::sync_physics_transform(registry_, delta);
			update::process_physics_events(registry_, delta);

			update::actor(registry_, delta);

			update::projectile(registry_, delta);

			update::sprite_animation(registry_, delta);
		}
	}

	auto Game::render(sf::RenderWindow& window) noexcept -> void
	{
		render::camera(registry_, window);

		render::floor(registry_, window);
		render::wall(registry_, window);
		render::door(registry_, window);

		render::enemy(registry_, window);
		render::player(registry_, window);
		render::health_mana_bar(registry_, window);

		render::projectile(registry_, window);

		if (g_physics_world_draw_on)
		{
			g_physics_world_draw.context = &window;
			b2World_Draw(utility::Physics::world_id, &g_physics_world_draw);
		}

		if (is_paused_)
		{
			pause_->render(window);
		}

		//
	}
}
