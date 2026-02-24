// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <config/types.hpp>
#include <game/render_layer.hpp>

#include <utility/string_hash.hpp>

namespace pd::config
{
	// ================================================
	// TILE
	// ================================================

	// 瓦片类型
	// 定义顺序可以不与RenderLayer顺序一致
	// 不过为了简化转换,这里直接设置对应值
	enum class TileType : std::uint64_t
	{
		FLOOR = RenderLayer::FLOOR,

		WALL = RenderLayer::WALL,
		WALL_DECORATION = RenderLayer::WALL_DECORATION,

		// todo: 门能否和其他瓦片叠加?

		FLOOR_DECORATION = RenderLayer::FLOOR_DECORATION,
		FLOOR_HOLE = RenderLayer::FLOOR_HOLE,
		LOW_TERRAIN = RenderLayer::LOW_TERRAIN,
		HIGH_TERRAIN = RenderLayer::HIGH_TERRAIN,
	};

	// todo: 瓦片应该如何设计?
	// 一块瓦片使用一整张纹理吗?
	// 还是一块瓦片使用纹理中的一部分?(瓦片集)
	// 瓦片只允许正方形吗?
	// 瓦片大小和房间地板大小是否需要一致?需要缩放吗?

	// 一块瓦片的配置数据
	class Tile final
	{
	public:
		// 纹理路径
		std::string texture_path;
		// 纹理位置
		// 如果一张纹理只有一个瓦片,则x=y=0
		// 否则需要指定瓦片在纹理中的位置
		int texture_x;
		int texture_y;
		// 纹理大小
		// 如果一张纹理只有一个瓦片,则width=height=瓦片尺寸
		// 否则需要指定瓦片在纹理中的尺寸
		int texture_width;
		int texture_height;
		// 瓦片类型
		TileType type;
	};

	// 瓦片集
	// 瓦片类型名称<->瓦片配置
	class TileSet final : public std::unordered_map<
				std::string,
				Tile,
				utility::StringHash,
				std::ranges::equal_to
			>
	{
	public:
		using unordered_map::unordered_map;
	};

	// 加载瓦片集配置
	[[nodiscard]] auto load_tileset(const std::filesystem::path& path) noexcept -> TileSet;

	// ================================================
	// ROOM
	// ================================================

	// 门的配置数据
	class DoorConfig final
	{
	public:
		// 连接的房间ID(在Level中的索引)
		std::string target_room_id;
		// TileSet中对应的瓦片ID
		std::string tile_id;
		// 生成位置(瓦片坐标)
		int tile_x;
		int tile_y;
		// 门所在方向
		DoorDirection direction;
		// 是否需要钥匙
		bool requires_key;
	};

	// 敌人生成点配置
	class EnemySpawn final
	{
	public:
		// 敌人ID
		std::string enemy_id;
		// 生成位置(瓦片坐标)
		int tile_x;
		int tile_y;
	};

	// 房间配置数据
	class Room final
	{
	public:
		// 房间ID(唯一标识)
		std::string id;
		// 房间类型
		RoomType room_type;

		// 房间宽度(瓦片数量)
		int width;
		// 房间高度(瓦片数量)
		int height;
		// 瓦片大小(像素)
		int tile_size;

		// 房间布局
		// 房间布局使用一个字符串阵列模拟,每一行表示房间的一行瓦片
		// 例如
		// "1###*###2",
		// "#-------#",
		// "#-------#",
		// "#-------#",
		// "*-------*",
		// "#-------#",
		// "#-------#",
		// "#-------#",
		// "3###*###4"
		// 这里的各种字符含义取决于下面的tile_mapping映射
		// todo: width*height决定了layout的尺寸,但是如果layout的行数或每行的长度不符合这个尺寸应该如何处理?
		//  或者说width/height没什么用,具体大小/布局还是得看layout(房间不一定是矩形?)
		std::vector<std::string> layout;

		// 瓦片映射
		// 例如
		//  "#": "wall_stone"
		//  "-": "floor_stone"
		//  "1": "wall_corner_tl"
		//  "2": "wall_corner_tr"
		//  "3": "wall_corner_bl"
		//  "4": "wall_corner_br"
		//  "*": "floor_stone"
		// 前面的字符是layout中使用的字符,后面的字符串是TileSet中对应的瓦片ID
		// 这里的*是门的位置,不过它们叠加在同一地块上,这里可以不做区分(也使用-表示也行)
		std::unordered_map<char, std::string> tile_mapping;

		// 门配置列表
		std::vector<DoorConfig> doors;

		// 敌人生成点列表
		std::vector<EnemySpawn> enemy_spawns;
	};

	// 房间集合
	// 房间ID -> 房间配置
	class RoomSet final : public std::unordered_map<
				std::string,
				Room,
				utility::StringHash,
				std::ranges::equal_to
			>
	{
	public:
		using unordered_map::unordered_map;
	};

	// 加载房间配置
	[[nodiscard]] auto load_room(const std::filesystem::path& path) noexcept -> Room;

	// ================================================
	// LEVEL
	// ================================================

	// 关卡配置数据
	class Level final
	{
	public:
		// 关卡ID
		std::string id;
		// 关卡名称
		std::string name;
		// 初始房间ID
		std::string starting_room_id;
		// 关卡包含的房间集合
		RoomSet rooms;
		// 使用的瓦片集路径
		std::string tileset_path;
	};

	// 关卡集合
	// 关卡ID -> 关卡配置
	class LevelSet final : public std::unordered_map<
				std::string,
				Level,
				utility::StringHash,
				std::ranges::equal_to
			>
	{
	public:
		using unordered_map::unordered_map;
	};

	// 加载关卡配置
	[[nodiscard]] auto load_level(const std::filesystem::path& path) noexcept -> Level;

	// ================================================
	// DUNGEON
	// ================================================

	// 地下城配置数据
	class Dungeon final
	{
	public:
		// 地下城ID
		std::string id;
		// 地下城名称
		std::string name;
		// 关卡顺序列表(关卡ID)
		std::vector<std::string> level_order;
		// 关卡集合
		LevelSet levels;
	};

	// 加载地下城配置
	[[nodiscard]] auto load_dungeon(const std::filesystem::path& path) noexcept -> Dungeon;
} // namespace pd::config
