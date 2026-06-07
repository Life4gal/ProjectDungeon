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
// #include <manager/event.hpp>
#include <manager/random.hpp>

// =========
// 物理世界特别处理

#include <utility/physics.hpp>
#include <component/collision.hpp>
#include <box2d/box2d.h>

// =========
// 测试用

#include <designer/level.hpp>
#include <designer/player.hpp>
#include <designer/projectile.hpp>
#include <factory/level.hpp>
#include <factory/player.hpp>
#include <factory/projectile.hpp>

#include <helper/camera.hpp>
#include <helper/room.hpp>
#include <helper/player_controller.hpp>
#include <helper/cheat.hpp>

#include <component/renderer.hpp>

// =========
// 更新

#include <update/graveyard.hpp>
#include <update/room_guardian.hpp>
#include <update/new_entity_tag.hpp>
#include <update/player_controller.hpp>
#include <update/move_behavior.hpp>
#include <update/physics_world.hpp>
#include <update/sync_physics_transform.hpp>
#include <update/process_physics_events.hpp>
#include <update/room_alive_check.hpp>
#include <update/limited_life.hpp>
#include <update/particle_emitter.hpp>
#include <update/dynamic_sprite.hpp>
#include <update/render_effect.hpp>

// =========
// 渲染

#include <render/collect_render_item.hpp>
#include <render/apply_render_effect.hpp>
#include <render/build_render_queue.hpp>
#include <render/commit_render_queue.hpp>
#include <render/particle.hpp>
#include <render/player_status.hpp>
#include <render/player_target_status.hpp>

// =========
// 依赖

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
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
			const auto [body_id] = registry.get<const component::collision::BodyId>(entity);
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
			registry.on_destroy<component::collision::BodyId>().connect<&on_destroy_physics_body>();
		}

		// 销毁物理世界
		auto destroy_physics_world(entt::registry& registry) noexcept -> void
		{
			auto& world_id = utility::Physics::world_id;

			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(world_id), "物理世界未创建");

			registry.on_destroy<component::collision::BodyId>().disconnect<&on_destroy_physics_body>();

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
	}

	auto Game::start_game() noexcept -> bool
	{
		// 进入地下城

		manager::Random::seed(123456);

		// 关卡
		const auto level = designer::Level::generate(8, 5, 15, 4, 2);
		factory::Level::create(registry_, level);

		// 玩家
		const auto player = designer::Player::test_character();
		const auto player_entity = factory::Player::spawn(registry_, player);
		helper::PlayerController::set_target(registry_, player_entity);

		// 进入起始房间
		helper::Room::enter(registry_, level.start_position.x, level.start_position.y);

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

		// TODO: 在合适的地方创建它们
		registry_.ctx().emplace<component::renderer::RenderItemSet>();
		registry_.ctx().emplace<component::renderer::RenderCommandQueue>();
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

		// 最后销毁物理世界
		destroy_physics_world(registry_);

		// TODO: 在合适的地方销毁它们
		registry_.ctx().erase<component::renderer::RenderItemSet>();
		registry_.ctx().erase<component::renderer::RenderCommandQueue>();
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
			if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>())
			{
				if (mbp->button == sf::Mouse::Button::Left)
				{
					// TODO: 蓝图持久化?
					const static auto projectile_blueprint = designer::Projectile::standard();

					if (helper::PlayerController::online(registry_))
					{
						const auto target = helper::PlayerController::target(registry_);
						const auto target_position = helper::PlayerController::screen_position(registry_);
						const auto direction = mbp->position - target_position;

						factory::Projectile::spawn(registry_, projectile_blueprint, target, sf::Vector2f{direction});
					}
				}
			}
			else if (const auto* kp = event.getIf<sf::Event::KeyPressed>())
			{
				using sf::Keyboard::Key;

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
					helper::PlayerController::left(registry_);
				}
				else if (kp->code == Key::D)
				{
					helper::PlayerController::right(registry_);
				}
				else if (kp->code == Key::W)
				{
					helper::PlayerController::up(registry_);
				}
				else if (kp->code == Key::S)
				{
					helper::PlayerController::down(registry_);
				}
				else if (kp->code == Key::Left or kp->code == Key::Right or kp->code == Key::Up or kp->code == Key::Down)
				{
					// TODO: 蓝图持久化?
					const static auto projectile_blueprint = designer::Projectile::standard();

					if (helper::PlayerController::online(registry_))
					{
						const auto target = helper::PlayerController::target(registry_);

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

						factory::Projectile::spawn(registry_, projectile_blueprint, target, direction);
					}
				}
				else if (kp->code == Key::E)
				{
					if (kp->control)
					{
						const auto target = helper::PlayerController::target(registry_);

						// 击杀所有敌人
						helper::Cheat::kill_all_enemy(registry_, target);
					}
					else
					{
						// 所有敌人生命值减半
						helper::Cheat::set_all_enemy_hp_percent(registry_, 0.5f);
					}
				}
			}
			else if (const auto* kr = event.getIf<sf::Event::KeyReleased>())
			{
				using sf::Keyboard::Key;

				// =====================
				// PLAYER_CONTROLLER
				// =====================
				if (kr->code == Key::A or kr->code == Key::D)
				{
					helper::PlayerController::stop_horizontal(registry_);
				}
				else if (kr->code == Key::W or kr->code == Key::S)
				{
					helper::PlayerController::stop_vertical(registry_);
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

			// 监控那些处于/不处于当前房间(相机区域)的实体
			update::room_guardian(registry_, delta);

			// 玩家控制器
			update::player_controller(registry_, delta);

			// (AI)移动行为
			update::move_behavior(registry_, delta);

			update::physics_world(registry_, delta);
			// b2World_GetBodyEvents -> b2BodyMoveEvent 的数据是该次b2World_Step的*旧*数据
			// 如果我们在 b2World_GetContactEvents/b2World_GetSensorEvents 可能直接修改物理体的 b2Transform 数据
			// 则*必须*先处理 b2World_GetBodyEvents 再处理 Contact/Sensor 相关事件
			// 否则处理 Contact/Sensor 时更新的 b2Transform(及其同步地数据) 将被*旧*数据覆盖 
			update::sync_physics_transform(registry_, delta);
			update::process_physics_events(registry_, delta);

			// 如果房间内敌人全部死亡则打开门
			update::room_alive_check(registry_, delta);

			// 有限生命
			update::limited_life(registry_, delta);

			// 粒子发射器
			update::particle_emitter(registry_, delta);

			// 动态精灵
			update::dynamic_sprite(registry_, delta);

			// 渲染特效
			update::render_effect(registry_, delta);

			// 检测新生成的实体是否处于当前房间(相机区域)
			// 在最后检测?这假定了上面的update不依赖这里设置的标签,否则逻辑至少延迟一帧(甚至错过)
			update::new_entity_tag(registry_, delta);
		}
	}

	auto Game::render(sf::RenderWindow& window) noexcept -> void
	{
		// ================
		// 绘制实体
		// 相机视图
		// ================

		const auto camera_area = helper::Camera::get_area(registry_);
		window.setView(sf::View{camera_area});

		// 收集渲染项
		render::collect_render_item(registry_);
		// 应用渲染效果
		render::apply_render_effect(registry_);
		// 构建渲染队列
		render::build_render_queue(registry_);
		// 提交渲染队列
		render::commit_render_queue(registry_, window);

		// 渲染粒子
		render::particle(registry_, window);

		// 物理调试绘制
		if (g_physics_world_draw_on)
		{
			g_physics_world_draw.context = &window;
			b2World_Draw(utility::Physics::world_id, &g_physics_world_draw);
		}

		// ================
		// 绘制HUD
		// 窗口视图
		// ================

		window.setView(window.getDefaultView());

		// 玩家状态
		render::player_status(registry_, window);
		// 玩家目标状态
		render::player_target_status(registry_, window);

		if (is_paused_)
		{
			pause_->render(window);
		}

		//
	}
}
