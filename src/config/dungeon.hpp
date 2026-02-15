// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

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
		DOOR = RenderLayer::DOOR,

		FLOOR_DECORATION = RenderLayer::FLOOR_DECORATION,
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
		using size_type = std::uint32_t;

		// 纹理路径
		std::string texture_path;
		// 纹理位置
		// 如果一张纹理只有一个瓦片,则x=y=0
		// 否则需要指定瓦片在纹理中的位置
		int x;
		int y;
		// 纹理大小
		// 如果一张纹理只有一个瓦片,则width=height=瓦片尺寸
		// 否则需要指定瓦片在纹理中的尺寸
		int width;
		int height;
		// 瓦片类型
		TileType tile_type;
	};

	// 瓦片集
	// 瓦片类型名称<->瓦片配置
	class TileSet final : public std::unordered_map<std::string, Tile, utility::StringHash, std::ranges::equal_to>
	{
	public:
		using unordered_map::unordered_map;
	};

	// ================================================
	// ROOM
	// ================================================

	// todo: 每个房间的设计

	// ================================================
	// LEVEL
	// ================================================

	// todo: 每个关卡(层级)的设计

	// ================================================
	// DUNGEON
	// ================================================

	// todo: 每个地牢的设计
} // namespace pd::config
