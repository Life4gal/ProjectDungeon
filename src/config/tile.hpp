// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <optional>

#include <config/config_reader.hpp>
#include <config/set.hpp>

namespace pd::config
{
	namespace detail
	{
		// "animation": "animation-id"
		class Animation
		{
		public:
			std::string animation_id;
		};

		// 通常物理碰撞体大小与纹理大小一致
		// 当然也支持碰撞区域小于(或者大于)纹理区域
		// todo: 如果动画帧的纹理大小不完全相同,此时碰撞区域是否也要跟着变化?目前仅取第一帧的纹理大小作为默认值(如果未指定)
		//  能否也是一个列表?
		// "collision": { "width": 16, "height": 16, "enable_contact_event": true, "is_sensor": true }
		class Collision
		{
		public:
			int collision_width;
			int collision_height;

			// 允许接触事件
			bool enable_contact_event;
			// 是否是感应器(感应器不阻止移动)
			bool is_sensor;
		};
	}

	// ================================================
	// 墙壁瓦片
	// ================================================

	// "wall-id": {
	//   "animation": "animation-id",
	//   "collision": { "width": 16, "height": 16 } // <- 墙壁不需要接触事件,也不是传感器
	// }
	class WallTile final : public detail::Animation, public detail::Collision {};

	// 墙壁瓦片集
	// 墙壁ID -> 墙壁瓦片配置
	// "walls": { "wall-id-1": { WallTile }, "wall-id-2": { WallTile }, ... }
	class WallTileSet final : public Set<WallTile>
	{
	public:
		using Set::Set;
	};

	// ================================================
	// 地板瓦片
	// ================================================

	// "floor-id": {
	//   "animation": "animation-id"
	// }
	class FloorTile final : public detail::Animation {};

	// 地板瓦片集
	// 地板ID -> 地板瓦片配置
	// "floors": { "floor-id-1": { FloorTile }, "floor-id-2": { FloorTile }, ... }
	class FloorTileSet final : public Set<FloorTile>
	{
	public:
		using Set::Set;
	};

	// ================================================
	// 装饰物瓦片
	// ================================================

	// "decoration-id": {
	//   "animation": "animation-id"
	// }
	class DecorationTile final : public detail::Animation {};

	// 装饰物瓦片集
	// 装饰物ID -> 装饰物瓦片配置
	// "decorations": { "decoration-id-1": { DecorationTile }, "decoration-id-2": { DecorationTile }, ... }
	class DecorationTileSet final : public Set<DecorationTile>
	{
	public:
		using Set::Set;
	};

	// ================================================
	// 触发器瓦片
	// ================================================

	// "trigger-id": {
	//   "animation": "animation-id",
	//   "collision": { "width": 16, "height": 16, "is_sensor": true } // <- 触发器需要接触事件,通常也是传感器(当然也可以不是)
	// }
	class TriggerTile final : public detail::Animation, public detail::Collision {};

	// 触发器瓦片集
	// 触发器ID -> 触发器瓦片配置
	// "triggers": { "trigger-id-1": { TriggerTile }, "trigger-id-2": { TriggerTile }, ... }
	class TriggerTileSet final : public Set<TriggerTile>
	{
	public:
		using Set::Set;
	};

	// ================================================
	// 钥匙瓦片
	// ================================================

	// "key-id": {
	//   "animation": "animation-id",
	//   "collision": { "width": 16, "height": 16 } // <- 钥匙本质就是一个触发器,不过在玩家接触后会销毁
	// }
	class KeyTile final : public detail::Animation, public detail::Collision {};

	// 钥匙集
	// "keys": { "key-id-1": { KeyTile }, "key-id-2": { KeyTile }, ... }
	class KeyTileSet final : public Set<KeyTile>
	{
	public:
		using Set::Set;
	};

	// ================================================
	// 门瓦片
	// ================================================

	// "door-id": {
	//   "animation": "animation-id",
	//   "collision": { "width": 16, "height": 16 } // <- 门本质就是一个触发器,不过它会在传感器和实体之间切换(关门/开门状态不同)
	// }
	// todo: 我们假定门瓦片动画反向播放符合关门动画,如此我们无需保存两份动画
	class DoorTile final : public detail::Animation, public detail::Collision {};

	// 门瓦片集
	// 门ID -> 门瓦片配置
	class DoorTileSet final : public Set<DoorTile>
	{
	public:
		using Set::Set;
	};

	// ================================================
	// 瓦片集
	// ================================================

	// 瓦片集
	// JSON:
	// { "tiles": "/path/to/tiles.json" }
	// { "tiles": { "walls": { ... }, "floors": { ... }, "decorations": { ... }, "triggers": { ... }, "keys": { ... }, "doors": { ... } } }
	class TileSet final
	{
	public:
		WallTileSet wall_tiles;
		FloorTileSet floor_tiles;
		DecorationTileSet decoration_tiles;
		TriggerTileSet trigger_tiles;
		KeyTileSet key_tiles;
		DoorTileSet door_tiles;
	};

	[[nodiscard]] auto load_tile_set_from_json(TileSet& tile_set, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_tile_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<TileSet>;
}
