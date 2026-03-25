// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <scene/scene.hpp>

#include <game/constants.hpp>

#include <manager/asset_fwd.hpp>

namespace pd::scene
{
	class Game final : public Scene
	{
	public:
		using option_type = game::GamePauseMenuOption;
		using asset_id_type = manager::AssetId;

	private:
		// 游戏字体
		asset_id_type font_id_;
		// 切换选项时音效
		asset_id_type sound_id_switch_option_;
		// 游戏音乐
		asset_id_type music_id_;

		// 选择的选项
		std::underlying_type_t<option_type> selected_option_value_;

		auto handle_event_pause(const sf::Event& event) noexcept -> void;

		// 开始一场游戏
		auto start_game() noexcept -> bool;

		// 从存档加载游戏
		auto load_game() noexcept -> bool;

		// 清理当前游戏并重新开始
		auto restart_game() noexcept -> void;

	public:
		using Scene::Scene;

		auto on_loaded() noexcept -> void override;

		auto on_initialized() noexcept -> void override;

		auto on_unloaded() noexcept -> void override;

		auto handle_event(const sf::Event& event) noexcept -> void override;

		auto update(sf::Time delta) noexcept -> void override;

		auto render(sf::RenderWindow& window) noexcept -> void override;
	};
}
