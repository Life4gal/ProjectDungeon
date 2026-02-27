// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/dungeon.hpp>

#include <config/config_reader.hpp>
#include <config/dungeon.hpp>

#include <components/dungeon.hpp>

#include <systems/helper/level.hpp>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::systems::helper
{
	auto Dungeon::load(entt::registry& registry, const std::filesystem::path& path) noexcept -> bool
	{
		const auto dungeon_json_data = config::ConfigReader::read_json(path);
		if (not dungeon_json_data.has_value())
		{
			return false;
		}

		auto dungeon_data = config::load_dungeon_from_json(*dungeon_json_data);
		if (not dungeon_data.has_value())
		{
			return false;
		}

		return load(registry, *std::move(dungeon_data));
	}

	auto Dungeon::load(entt::registry& registry, config::Dungeon dungeon) noexcept -> bool
	{
		using namespace components;

		// 卸载当前地下城(如果有)
		unload(registry);

		const auto& first_level_id = dungeon.level_order.front();
		const auto& level = dungeon.level_set.at(first_level_id);

		// 创建关卡
		const auto level_entity = Level::create(registry, level);

		if (level_entity == entt::null)
		{
			// 创建失败
			return false;
		}

		// 保存关卡实体
		registry.ctx().emplace<dungeon::Level>(level_entity);

		// 保存配置
		registry.ctx().emplace<dungeon::Dungeon>(std::move(dungeon));

		return true;
	}

	auto Dungeon::unload(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		if (not registry.ctx().contains<dungeon::Dungeon>())
		{
			return;
		}

		// 销毁关卡
		const auto [level_entity] = registry.ctx().get<const dungeon::Level>();
		Level::destroy(registry, level_entity);

		// 销毁关卡实体
		registry.ctx().erase<dungeon::Level>();

		// 移除配置
		registry.ctx().erase<dungeon::Dungeon>();
	}
}
