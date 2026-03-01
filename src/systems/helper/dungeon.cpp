// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/dungeon.hpp>

#include <config/config_reader.hpp>
#include <config/dungeon.hpp>

#include <components/tags.hpp>
#include <components/dungeon.hpp>

#include <systems/helper/player.hpp>
#include <systems/helper/level.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::systems::helper
{
	auto Dungeon::load(entt::registry& registry, const std::filesystem::path& path) noexcept -> bool
	{
		SPDLOG_INFO("正在加载地下城文件[{}]...", path.string());

		const auto dungeon_json_data = config::ConfigReader::read_json(path);
		if (not dungeon_json_data.has_value())
		{
			return false;
		}

		SPDLOG_INFO("正在加载地下城数据...");

		auto dungeon_data = config::load_dungeon_from_json(*dungeon_json_data);
		if (not dungeon_data.has_value())
		{
			return false;
		}

		SPDLOG_INFO("正在创建地下城...");

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
			return false;
		}

		// 保存关卡实体
		registry.ctx().emplace<dungeon::Level>(level_entity);

		// 保存配置
		registry.ctx().emplace<dungeon::Dungeon>(std::move(dungeon));

		// 创建一个测试用实体
		// 64 / 16 == 4
		const auto player_entity = Player::spawn(registry, sf::Vector2f{500, 500}, sf::Vector2f{4, 4});
		if (player_entity == entt::null)
		{
			return false;
		}
		Level::join(registry, level_entity, player_entity);

		return true;
	}

	auto Dungeon::unload(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		if (not registry.ctx().contains<dungeon::Dungeon>())
		{
			return;
		}

		// 销毁测试用实体
		Player::kill(registry);

		// 销毁关卡
		const auto [level_entity] = registry.ctx().get<const dungeon::Level>();
		Level::destroy(registry, level_entity);

		// 销毁关卡实体
		registry.ctx().erase<dungeon::Level>();

		// 移除配置
		registry.ctx().erase<dungeon::Dungeon>();
	}

	auto Dungeon::on_update(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		Level::on_update(registry, level(registry));
	}

	auto Dungeon::on_trigger_contact(entt::registry& registry, const entt::entity trigger_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::trigger>(trigger_entity));

		Level::on_trigger_contact(registry, level(registry), trigger_entity, other_entity);
	}

	auto Dungeon::on_key_contact(entt::registry& registry, const entt::entity key_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::key>(key_entity));

		Level::on_key_contact(registry, level(registry), key_entity, other_entity);
	}

	auto Dungeon::on_locked_door_contact(entt::registry& registry, const entt::entity door_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::door>(door_entity));

		Level::on_locked_door_contact(registry, level(registry), door_entity, other_entity);
	}

	auto Dungeon::on_unlocked_door_contact(entt::registry& registry, const entt::entity door_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::door>(door_entity));

		Level::on_unlocked_door_contact(registry, level(registry), door_entity, other_entity);
	}

	auto Dungeon::level(entt::registry& registry) noexcept -> entt::entity
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<dungeon::Level>());

		return registry.ctx().get<const dungeon::Level>().entity;
	}
}
