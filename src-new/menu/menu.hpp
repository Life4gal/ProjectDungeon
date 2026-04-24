// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace sf
{
	class Event;
	class RenderWindow;
}

namespace pd::menu
{
	enum class Action : std::uint8_t
	{
		// 上移
		UP,
		// 下移
		DOWN,
		// 左移
		LEFT,
		// 右移
		RIGHT,

		// 确定
		CONFIRM,
		// 取消
		CANCEL,

		// 计数-不要使用
		COUNT,
		// 无动作-用于默认值
		NONE,
	};

	class ActionMapper
	{
	public:
		ActionMapper() noexcept;

		ActionMapper(const ActionMapper&) noexcept = delete;
		auto operator=(const ActionMapper&) noexcept -> ActionMapper& = delete;

		ActionMapper(ActionMapper&&) noexcept = default;
		auto operator=(ActionMapper&&) noexcept -> ActionMapper& = default;

		virtual ~ActionMapper() noexcept;

		[[nodiscard]] virtual auto map(sf::Keyboard::Key key) const noexcept -> Action;
	};

	class Menu
	{
	public:
		//

	private:
		// 鼠标移动操作
		virtual auto on_cursor_move(int x, int y) noexcept -> void = 0;
		// 鼠标点击操作
		virtual auto on_cursor_click(int x, int y) noexcept -> void = 0;
		// 键盘操作
		virtual auto on_action(Action action) noexcept -> void = 0;
		// 帧更新
		virtual auto on_update(sf::Time delta) noexcept -> void = 0;
		// 渲染
		virtual auto on_render(sf::RenderWindow& window) noexcept -> void = 0;

	public:
		explicit Menu() noexcept;

		Menu(const Menu&) noexcept = delete;
		auto operator=(const Menu&) noexcept -> Menu& = delete;

		Menu(Menu&&) noexcept = default;
		auto operator=(Menu&&) noexcept -> Menu& = default;

		virtual ~Menu() noexcept;

		// 处理事件(返回事件是否被响应/处理)
		auto handle_event(const sf::Event& event) noexcept -> bool;

		// 帧更新
		auto update(sf::Time delta) noexcept -> void;

		// 渲染
		auto render(sf::RenderWindow& window) noexcept -> void;
	};
}
