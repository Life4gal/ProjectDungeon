// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <blueprint/room.hpp>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Time.hpp>

#include <box2d/id.h>

namespace pd::components::room
{
	namespace door
	{
		// ReSharper disable once CppInconsistentNaming
		using Direction = blueprint::DoorDirection;

		// ReSharper disable once CppInconsistentNaming
		using Type = blueprint::DoorType;

		class Locked {};

		enum class State : std::uint8_t
		{
			// 已关闭
			CLOSED,
			// 正在关闭
			CLOSING,
			// 已开启
			OPENED,
			// 正在开启
			OPENING,
		};

		// 开关门动画计时器
		// 仅在播放动画时存在
		class Timer final
		{
		public:
			sf::Time timer;
		};

		// 物理体
		// 仅在激活时存在
		class Physics final
		{
		public:
			// 门
			b2ShapeId door;
			// 感应区
			b2ShapeId sensor;
			// 阻挡(防止玩家走到地图外部)
			b2ShapeId blocker;
		};
	}

	namespace item
	{
		//
	}

	namespace chest
	{
		enum class Type : std::uint8_t
		{
			// 钱币
			COIN,
			// 物品
			ITEM,
			// 装备
			EQUIPMENT,
			// 食物/药水
			CONSUMABLE,

			// 爆炸陷阱
			EXPLOSION_TRAP,
			// 落石陷阱
			ROCKFALL_TRAP,
		};

		// 动画计时器
		// 仅在播放动画时存在
		class Timer final
		{
		public:
			sf::Time timer;
		};
	}

	namespace blood_stain
	{
		//
	}

	namespace corpse
	{
		// 死亡时间
		// 死亡时间越大越后渲染
		class Time final
		{
		public:
			sf::Time time;
		};
	}

	namespace destroyable
	{
		//
	}

	// ==========================================
	// 上下文组件
	// ==========================================

	//

	// ==========================================
	// 实体组件
	// ==========================================

	// 房间布局
	class Layout final
	{
	public:
		using layout_type = blueprint::Room::layout_type;

		layout_type layout;
	};

	// 房间类型
	class Type final
	{
	public:
		using room_type = blueprint::RoomType;

		room_type type;
	};

	// 房间坐标
	class Position final
	{
	public:
		std::uint32_t x;
		std::uint32_t y;
	};

	// 房间已访问
	// 已访问的房间会显示在小地图上
	class Visited final {};

	// 房间已清理
	class Cleared final {};

	// 房间内所有门实体
	class Doors final
	{
	public:
		std::vector<entt::entity> entities;
	};

	// 房间内所有物品实体
	class Items final
	{
	public:
		std::vector<entt::entity> entities;
	};

	// 房间内所有箱子实体
	class Chests final
	{
	public:
		std::vector<entt::entity> entities;
	};

	// 房间内所有血迹实体
	class BloodStains final
	{
	public:
		std::vector<entt::entity> entities;
	};

	// 房间内所有尸体实体
	class Corpses final
	{
	public:
		std::vector<entt::entity> entities;
	};

	// 房间内所有可破坏物实体
	class Destroyable final
	{
	public:
		std::vector<entt::entity> entities;
	};
}
