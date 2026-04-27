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

// =========
// 物理世界特别处理
#include <utility/physics.hpp>
#include <component/physics_body.hpp>
#include <box2d/box2d.h>

// =========
// 测试用

#include <factory/room.hpp>

// =========
// 更新

#include <update/physics_world.hpp>
#include <update/sprite_animation.hpp>

// =========
// 渲染

#include <render/floor.hpp>
#include <render/wall.hpp>
#include <render/player.hpp>

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
				auto* window = static_cast<sf::RenderWindow*>(context);

				sf::ConvexShape polygon{};

				polygon.setPointCount(vertex_count);
				for (int i = 0; i < vertex_count; ++i)
				{
					const auto& vertex = vertices[i];

					const auto point = Physics::from_physics(vertex);
					polygon.setPoint(i, point);
				}

				polygon.setFillColor(sf::Color::Transparent);
				polygon.setOutlineColor(rgb_to_rgba(color));
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
						auto* window = static_cast<sf::RenderWindow*>(context);

						sf::ConvexShape polygon{};

						const b2Vec2 origin = transform.p;
						polygon.setPointCount(vertex_count);
						for (int i = 0; i < vertex_count; ++i)
						{
							const auto& vertex = vertices[i];
							const auto rotated = b2RotateVector(transform.q, vertex);
							const auto position = origin + rotated;

							const auto point = Physics::from_physics(position);
							polygon.setPoint(i, point);
						}

						const auto sf_color = rgb_to_rgba(color);
						const auto sf_color_half_transparent = sf::Color{sf_color.r, sf_color.g, sf_color.b, 128};
						polygon.setFillColor(sf_color_half_transparent);
						polygon.setOutlineColor(sf_color);
						polygon.setOutlineThickness(1);

						window->draw(polygon);
					};
			draw.DrawCircleFcn = [](const b2Vec2 center, const float radius, const b2HexColor color, void* context) noexcept -> void
			{
				auto* window = static_cast<sf::RenderWindow*>(context);

				sf::CircleShape circle{Physics::from_physics(radius)};
				circle.setPosition(Physics::from_physics(center));

				const auto sf_color = rgb_to_rgba(color);
				circle.setFillColor(sf::Color::Transparent);
				circle.setOutlineColor(sf_color);
				circle.setOutlineThickness(2);

				window->draw(circle);
			};
			draw.DrawSolidCircleFcn = [](const b2Transform transform, const float radius, const b2HexColor color, void* context) noexcept -> void
			{
				auto* window = static_cast<sf::RenderWindow*>(context);

				sf::CircleShape circle{Physics::from_physics(radius)};
				circle.setPosition(Physics::from_physics(transform.p));

				const auto sf_color = rgb_to_rgba(color);
				const auto sf_color_half_transparent = sf::Color{sf_color.r, sf_color.g, sf_color.b, 128};
				circle.setFillColor(sf_color_half_transparent);
				circle.setOutlineColor(sf_color);
				circle.setOutlineThickness(1);

				window->draw(circle);
			};
			draw.DrawSolidCapsuleFcn = [](const b2Vec2 p1, const b2Vec2 p2, const float radius, const b2HexColor color, void* context) noexcept -> void
			{
				auto* window = static_cast<sf::RenderWindow*>(context);

				const auto direction = p2 - p1;
				const auto length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
				const auto angle = std::atan2(direction.y, direction.x);

				sf::RectangleShape rect{{Physics::from_physics(length), Physics::from_physics(radius * 2)}};
				rect.setOrigin({0, Physics::from_physics(radius)});
				rect.setPosition(Physics::from_physics(p1));
				rect.setRotation(sf::radians(angle));

				sf::CircleShape end1{Physics::from_physics(radius)};
				end1.setOrigin({Physics::from_physics(radius), Physics::from_physics(radius)});
				end1.setPosition(Physics::from_physics(p1));

				sf::CircleShape end2{Physics::from_physics(radius)};
				end2.setOrigin({Physics::from_physics(radius), Physics::from_physics(radius)});
				end2.setPosition(Physics::from_physics(p2));

				const auto sf_color = rgb_to_rgba(color);
				const auto sf_color_half_transparent = sf::Color{sf_color.r, sf_color.g, sf_color.b, 128};
				rect.setFillColor(sf_color_half_transparent);
				end1.setFillColor(sf_color_half_transparent);
				end2.setFillColor(sf_color_half_transparent);

				window->draw(rect);
				window->draw(end1);
				window->draw(end2);
			};
			draw.DrawSegmentFcn = [](const b2Vec2 p1, const b2Vec2 p2, const b2HexColor color, void* context) noexcept -> void
			{
				auto* window = static_cast<sf::RenderWindow*>(context);

				const std::array<sf::Vertex, 2> line
				{{
						{.position = Physics::from_physics(p1), .color = rgb_to_rgba(color), .texCoords = {}},
						{.position = Physics::from_physics(p2), .color = rgb_to_rgba(color), .texCoords = {}},
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

		// 测试用
		constexpr int TileWidth = 64;
		constexpr int TileHeight = 64;
		constexpr int TileOriginX = TileWidth / 2;
		constexpr int TileOriginY = TileHeight / 2;
		blueprint::Room g_test_room
		{
				.offset_x = 0,
				.offset_y = 0,
				.floors =
				{
						// 第一行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第二行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 1, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第三行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 2, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第四行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 3, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第五行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 4, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第六行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 5, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第七行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 6, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第八行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 7, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						// 第九行
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 0, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 1, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 2, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 3, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 4, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 5, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 6, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 7, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 8, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
						{
								.transform = {.x = TileWidth + TileOriginX + TileWidth * 9, .y = TileHeight + TileOriginY + TileHeight * 8, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
						},
				},
				.walls =
				{
						// 左上
						{
								.transform = {.x = TileOriginX, .y = TileOriginY, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/wall.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
								.physics_body = {.type = blueprint::PhysicsBodyType::STATIC, .is_bullet = false},
								// clang-format off
								.physics_shape = {.def = {.material = {.friction = 0, .restitution = 0}, .is_sensor = false, .enable_sensor_events = false, .enable_contact_events = false}, .width = TileWidth, .height =TileHeight},
								// clang-format on
						},
						// 右上
						{
								.transform = {.x = TileOriginX + TileWidth * (9 + 2), .y = TileOriginY, .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/wall.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
								.physics_body = {.type = blueprint::PhysicsBodyType::STATIC, .is_bullet = false},
								// clang-format off
								.physics_shape = {.def = {.material = {.friction = 0, .restitution = 0}, .is_sensor = false, .enable_sensor_events = false, .enable_contact_events = false}, .width = TileWidth, .height = TileHeight},
								// clang-format on
						},
						// 左下
						{
								.transform = {.x = TileOriginX, .y = TileOriginY + TileHeight * (8 + 2), .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/wall.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
								.physics_body = {.type = blueprint::PhysicsBodyType::STATIC, .is_bullet = false},
								// clang-format off
								.physics_shape = {.def = {.material = {.friction = 0, .restitution = 0}, .is_sensor = false, .enable_sensor_events = false, .enable_contact_events = false}, .width = TileWidth, .height = TileHeight},
								// clang-format on
						},
						// 右下
						{
								.transform = {.x = TileOriginX + TileWidth * (9 + 2), .y = TileOriginY + TileHeight * (8 + 2), .scale_x = 1, .scale_y = 1, .rotation = 0},
								.sprite = {.texture = "./assets/tileset/wall.png", .x = 0, .y = 0, .width = TileWidth, .height = TileHeight, .origin_x = TileOriginX, .origin_y = TileOriginY},
								.physics_body = {.type = blueprint::PhysicsBodyType::STATIC, .is_bullet = false},
								// clang-format off
								.physics_shape = {.def = {.material = {.friction = 0, .restitution = 0}, .is_sensor = false, .enable_sensor_events = false, .enable_contact_events = false}, .width = TileWidth, .height = TileHeight},
								// clang-format on
						},
				},
		};
	}

	auto Game::start_game() noexcept -> bool
	{
		// 进入地下城

		factory::Room::create(registry_, g_test_room);

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

		create_physics_world(registry_);

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
		factory::Room::destroy(registry_);

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
				if (kp->code == sf::Keyboard::Key::Escape)
				{
					is_paused_ = true;
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
			update::physics_world(registry_, delta);
			update::sprite_animation(registry_, delta);
		}
	}

	auto Game::render(sf::RenderWindow& window) noexcept -> void
	{
		render::floor(registry_, window);
		render::wall(registry_, window);
		render::player(registry_, window);

		g_physics_world_draw.context = &window;
		b2World_Draw(utility::Physics::world_id, &g_physics_world_draw);

		if (is_paused_)
		{
			pause_->render(window);
		}

		//
	}
}
