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
	class MainMenu final : public Scene
	{
	public:
		using option_type = game::MainMenuOption;
		using asset_id_type = manager::AssetId;

	private:
		// 主菜单字体
		asset_id_type font_id_;
		// 切换选项时音效
		asset_id_type sound_id_switch_option_;
		// 主菜单音乐
		asset_id_type music_id_;

		// 选择的选项
		std::underlying_type_t<option_type> selected_option_value_;

		// =======================
		// HANDLE_EVENT
		// =======================

		auto handle_event_main(const sf::Event& event) noexcept -> void;

		// =======================
		// RENDER
		// =======================

		auto render_main(sf::RenderWindow& window) const noexcept -> void;

	public:
		explicit MainMenu(Game& game) noexcept;

		auto on_loaded() noexcept -> void override;

		auto on_initialized() noexcept -> void override;

		auto on_unloaded() noexcept -> void override;

		auto handle_event(const sf::Event& event) noexcept -> void override;

		auto update(sf::Time delta) noexcept -> void override;

		auto render(sf::RenderWindow& window) noexcept -> void override;
	};
}
