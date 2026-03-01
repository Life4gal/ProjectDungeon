// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <optional>

#include <config/config_reader.hpp>
#include <config/set.hpp>
#include <config/types.hpp>
#include <config/position.hpp>

namespace pd::config
{
	// ================================================
	// 房间
	// ================================================

	// 墙壁的配置数据
	class Wall final
	{
	public:
		// WallTileSet::key
		std::string tile_id;
	};

	// 地板的配置数据
	class Floor final
	{
	public:
		// FloorTileSet::key
		std::string tile_id;
	};

	// 装饰物的配置数据
	class Decoration final
	{
	public:
		// DecorationTileSet::key
		std::string tile_id;
	};

	// 触发器的配置数据
	class Trigger final
	{
	public:
		// TriggerTileSet::key
		std::string tile_id;
	};

	// 门配置数据
	class Door final
	{
	public:
		// DoorTileSet::key
		std::string tile_id;

		// 连接的房间ID(穿过该门进入的房间)
		std::string target_room;
		// 开启该门的钥匙名称(如果为空则表示不需要钥匙?)
		std::string key;
		// 门的所在方向(这控制了门感应/碰撞区域的位置)
		DoorDirection direction;
	};

	// 房间配置数据
	// JSON:
	// "room-id": { 
	//   "name": "房间名称", 
	//   "type": "STARTING/NORMAL/TREASURE/KEY/BOSS/EXIT", 
	//   "tile_size": 32, 
	//   "walls": { "wall-id-1": [ { "x": 0, "y": 0 }, ... ], ... }, 
	//   "floors": { "floor-id-1": [ { "x": 0, "y": 0 }, ... ], ... }, 
	//   "decorations": { "decoration-id-1": [ { "x": 0, "y": 0 }, ... ], ... }, 
	//   "triggers": { "trigger-id-1": [ { "x": 0, "y": 0 }, ... ], ... }, 
	//   "doors": { "door-id-1": [ { "x": 0, "y": 0, "target_room": "room-id", "key": "key-name", "direction": "UP/DOWN/LEFT/RIGHT" }, ... ], ... }, 
	//   "player_spawn_point": { "tile_x": 0, "tile_y": 0 }, 
	//   "enemy_spawn_points": [ { "x": 0, "y": 0 }, ... ], 
	//   "npc_spawn_points": [ { "x": 0, "y": 0 }, ... ], 
	//   "item_spawn_points": [ { "x": 0, "y": 0 }, ... ] 
	// } 
	// "room-id": "/path/to/room-config.json"
	class Room final
	{
	public:
		// 房间名称
		std::string name;
		// 房间类型
		RoomType type;
		// 瓦片大小(像素)
		int tile_size;
		// 房间宽度
		int width;
		// 房间高度
		int height;

		// 下面这些映射表的配置格式大致为(以json格式为例)
		// "xxx": { "name": [ { "x": 0, "y": 0 }, ... ] }
		// "doors": { "name": [ { "x": 0, "y": 0, "target_room": "room-id", "key": "key-name", "direction": "UP" }, ... ] }

		// 墙壁位置与瓦片的映射表
		std::unordered_map<Position, Wall> wall_tiles;
		// 地板位置与瓦片的映射表
		std::unordered_map<Position, Floor> floor_tiles;
		// 装饰物位置与瓦片的映射表
		std::unordered_map<Position, Decoration> decoration_tiles;
		// 触发器位置与瓦片的映射表
		std::unordered_map<Position, Trigger> trigger_tiles;
		// 门位置与门配置的映射表
		std::unordered_map<Position, Door> door_tiles;

		// 玩家生成点(如果是初始房间)
		Position player_spawn_point;
		// 敌人生成点列表(如果有)
		std::vector<Position> enemy_spawn_points;
		// NPC生成点列表(如果有)
		std::vector<Position> npc_spawn_points;
		// 道具生成点列表(如果有)(固定道具,不算敌人爆出的道具)
		std::vector<Position> item_spawn_points;
	};

	// 房间集
	// 房间ID -> 房间配置
	// JSON:
	// { "rooms": "/path/to/rooms.json" }
	// { "rooms": { "room-id-1": { Room }, "room-id-2": { Room }, ... } }
	class RoomSet final : public Set<Room>
	{
	public:
		using Set::Set;
	};

	[[nodiscard]] auto load_room_from_json(Room& room, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_room_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Room>;

	[[nodiscard]] auto load_room_set_from_json(RoomSet& room_set, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_room_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<RoomSet>;
}
