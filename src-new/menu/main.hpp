// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <menu/menu.hpp>

#include <manager/asset_fwd.hpp>

namespace pd::menu
{
	class Main final : public Menu
	{
	public:
		using asset_id_type = manager::AssetId;

		enum class Option : std::uint8_t
		{
			// 随机数种子
			RANDOM_SEED = 0,
			// 玩家名称
			PLAYER_NAME,
			// 继续游戏
			CONTINUE,
			// 开始新游戏
			START,
			// 选项
			OPTION,
			// 退出游戏
			QUIT,

			COUNT,
		};

	private:
		// 主菜单字体
		asset_id_type font_id_;
		// 主菜单音效-切换选项
		asset_id_type sound_id_switch_option_;

		// 选择的选项
		std::underlying_type_t<Option> selected_option_value_;

		auto on_cursor_move(int x, int y) noexcept -> void override;
		auto on_cursor_click(int x, int y) noexcept -> void override;
		auto on_action(Action action) noexcept -> void override;
		auto on_update(sf::Time delta) noexcept -> void override;
		auto on_render(sf::RenderWindow& window) noexcept -> void override;

	public:
		explicit Main() noexcept;

		Main(const Main&) noexcept = delete;
		auto operator=(const Main&) noexcept -> Main& = delete;

		Main(Main&&) noexcept = default;
		auto operator=(Main&&) noexcept -> Main& = default;

		~Main() noexcept override;
	};
}
