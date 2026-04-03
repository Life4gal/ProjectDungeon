// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/floor.hpp>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::components::floor
{
	class RoomInfo;
}

namespace pd::systems
{
	class Floor final
	{
	public:
		using position_type = sf::Vector2u;
		using info_type = components::floor::RoomInfo;

		// =============================================
		// 事件响应
		// =============================================

		// 订阅事件
		static auto subscribe_events(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe_events(entt::registry& registry) noexcept -> void;

	private:
		// 创建楼层
		static auto on_generate_request(entt::registry& registry, const events::floor::GenerateRequest& event) noexcept -> void;

		// 进入房间
		static auto on_enter_room(entt::registry& registry, const events::floor::Entered& event) noexcept -> void;

		// 清理房间
		static auto on_clear_room(entt::registry& registry, const events::floor::Cleared& event) noexcept -> void;

		// 离开房间
		static auto on_leave_room(entt::registry& registry, const events::floor::Left& event) noexcept -> void;

	public:
		// =============================================
		// 创建/销毁上下文
		// =============================================

		static auto create(entt::registry& registry) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;

		// // =============================================
		// // 内容获取
		// // =============================================
		//
		// // 获取该层级房间总数量
		// [[nodiscard]] static auto get_room_count(entt::registry& registry) noexcept -> std::uint32_t;
		//
		// // 获取当前所在房间坐标
		// [[nodiscard]] static auto get_current_position(entt::registry& registry) noexcept -> position_type;
		//
		// // ===========
		// // 目标位置
		// // ===========
		//
		// // 获取目标位置房间信息
		// // 如果目标位置不存在房间则返回nullptr
		// [[nodiscard]] static auto get_room_info(entt::registry& registry, position_type position) noexcept -> info_type*;
		//
		// // 确定目标位置是否存在房间
		// [[nodiscard]] static auto has_room(entt::registry& registry, position_type position) noexcept -> bool;
		//
		// // 统计某个位置四周已放置(非NONE)房间的数量
		// // 这也可以确定门的数量
		// [[nodiscard]] static auto count_placed_neighbors(entt::registry& registry, position_type position) noexcept -> std::uint32_t;
		//
		// // 获取某个位置四周已放置(非NONE)房间的坐标
		// [[nodiscard]] static auto get_placed_neighbors(entt::registry& registry, position_type position) noexcept -> std::vector<position_type>;
		//
		// // 获取某个位置四周未放置(NONE)房间的坐标
		// [[nodiscard]] static auto get_unplaced_neighbors(entt::registry& registry, position_type position) noexcept -> std::vector<position_type>;
		//
		// // // 获取所有已放置位置四周未放置(NONE)房间的坐标
		// // [[nodiscard]] static auto get_frontier(entt::registry& registry, std::span<const position_type> placed) noexcept -> std::vector<position_type>;
		//
		// // ===========
		// // 当前位置
		// // ===========
		//
		// // 获取当前房间信息
		// [[nodiscard]] static auto get_current_room_info(entt::registry& registry) noexcept -> info_type*;
		//
		// // 统计当前房间四周已放置(非NONE)房间的数量
		// [[nodiscard]] static auto count_current_placed_neighbors(entt::registry& registry) noexcept -> std::uint32_t;
		//
		// // 获取当前房间四周已放置(非NONE)房间的坐标
		// [[nodiscard]] static auto get_current_placed_neighbors(entt::registry& registry) noexcept -> std::vector<position_type>;
		//
		// // 获取当前房间四周未放置(NONE)房间的坐标
		// [[nodiscard]] static auto get_current_unplaced_neighbors(entt::registry& registry) noexcept -> std::vector<position_type>;
	};
}
