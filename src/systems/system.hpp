// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Time.hpp>

namespace sf
{
	class RenderWindow;
}

namespace pd::systems
{
	// 这个类其实没有必要
	// 1.各种系统不一定有全部三个接口
	// 2.各个接口的实现属于完全不同领域,完全可以拆分实现
	// 不过为了统一接口,依然定义该基类
	// 这里完全可以使用动态多态,不过也无所谓 :)
	template<typename Derived>
	class System
	{
		[[nodiscard]] constexpr auto rep() noexcept -> Derived&
		{
			return static_cast<Derived&>(*this);
		}

	public:
		auto on_loaded(entt::registry& registry) noexcept -> void
		{
			rep().do_loaded(registry);
		}

		auto on_initialized(entt::registry& registry) noexcept -> void
		{
			rep().do_initialized(registry);
		}

		auto on_unloaded(entt::registry& registry) noexcept -> void
		{
			rep().do_unloaded(registry);
		}

		auto update(entt::registry& registry, const sf::Time delta) noexcept -> void
		{
			rep().do_update(registry, delta);
		}

		auto render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
		{
			rep().do_render(registry, window);
		}
	};
}
