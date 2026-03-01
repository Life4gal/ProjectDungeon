// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <functional>
#include <vector>

#include <config/types.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::config
{
	class Room;
}

namespace pd::components
{
	// 房间
	namespace room
	{
		// todo: 钥匙相关内容应该放在上下文中吗?还是说放在实体组件中?

		// ==========================================
		// 上下文组件
		// ==========================================

		// 

		// ==========================================
		// 实体组件
		// ==========================================

		// 对config::Room的引用
		class Room final
		{
		public:
			std::reference_wrapper<const config::Room> room;
		};

		// 房间内墙壁瓦片实体
		// 对应config::Room::wall_tiles
		class Walls final
		{
		public:
			std::vector<entt::entity> entities;
		};

		// 房间内地板瓦片实体
		// 对应config::Room::floor_tiles
		class Floors final
		{
		public:
			std::vector<entt::entity> entities;
		};

		// 房间内装饰物瓦片实体
		// 对应config::Room::decoration_tiles
		class Decorations final
		{
		public:
			std::vector<entt::entity> entities;
		};

		// 房间内触发器瓦片实体
		// 对应config::Room::trigger_tiles
		class Triggers final
		{
		public:
			std::vector<entt::entity> entities;
		};

		// 房间内门瓦片实体
		// 对应config::Room::door_tiles
		class Doors final
		{
		public:
			std::vector<entt::entity> entities;
		};

		// 房间内敌人实体
		class Enemies final
		{
		public:
			std::vector<entt::entity> entities;
		};

		// 房间居中偏移
		// 该偏移确保房间整体处于屏幕中心区域
		class CenteringOffset final
		{
		public:
			float x;
			float y;
		};

		// 当前房间是否被清空
		// 房间满足清空条件则会添加该标签
		// 同时,如果房间含义该标记,进入该房间不再会进行生成(例如进入别的房间后再次回到该房间)
		class Cleared final {};
	}

	// 房间墙壁
	namespace wall
	{
		// transform 
		// animation (可选,如果无需更新的话)
		// render
		// physics_body 
	}

	// 房间地板
	namespace floor
	{
		// transform 
		// animation (可选,如果无需更新的话)
		// render 
	}

	// 房间装饰物
	namespace decoration
	{
		// transform
		// animation (可选,如果无需更新的话)
		// render 
	}

	// 房间触发器
	namespace trigger
	{
		// transform 
		// animation (可选,如果无需更新的话)
		// render
		// physics_body 
	}

	// 房间门
	namespace door
	{
		// transform
		// render
		// animation (可选,如果无需更新的话)
		// physics_body 

		// 标记当前门是否锁住
		// 如果未锁住则不存在该数据
		class Locked final {};

		// 标记当前门的钥匙
		// 如果是null则表示不需要钥匙
		// 即使有钥匙也需要消灭当前房间所有敌人才能解锁门
		// 也就是说需要同时满足拥有钥匙(如果需要)和消灭敌人这两个条件才能解锁门
		class Key final
		{
		public:
			entt::entity key;
		};

		// 门的方向
		class Direction final
		{
		public:
			config::DoorDirection direction;
		};

		// 门连接的下一个房间(使用房间ID字符串)
		class TargetRoom final
		{
		public:
			std::string room_id;
		};

		// 门的碰撞体尺寸(物理世界单位,半宽半高)
		// box2d不允许简单地将shape从传感器改为一般碰撞体(反之亦然),只能销毁原有shape并创建新shape
		// 所以我们需要保存门的碰撞体大小,以避免解锁/锁定门时需要再次指定门信息
		class CollisionSize final
		{
		public:
			float half_width;
			float half_height;
		};
	}
}
