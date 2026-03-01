// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <functional>
#include <unordered_map>

#include <utility/string_hash.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::config
{
	class Level;
}

namespace pd::components::level
{
	// ==========================================
	// 上下文组件
	// ==========================================

	//

	// ==========================================
	// 实体组件
	// ==========================================

	// 当前关卡对config::Level的引用
	class Level final
	{
	public:
		std::reference_wrapper<const config::Level> level;
	};

	// 所有房间的实体列表
	// 我们会保存所有房间的实体(因为我们会重复进入一些房间)
	// 在结束一个level时会销毁所有房间实体
	class Rooms final
	{
	public:
		// 我们使用map而不是vector来保存,以便快速检索需要的实体
		// key: 房间ID, value: 房间实体
		std::unordered_map<std::string, entt::entity, utility::StringHash, std::equal_to<>> entities;
	};

	// 当前房间实体
	class Room final
	{
	public:
		entt::entity entity;
	};

	// 当前关卡已经收集的钥匙列表
	class CollectedKeys final
	{
	public:
		std::vector<std::string> keys;
	};
}
