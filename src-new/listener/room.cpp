// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/room.hpp>

#include <manager/event.hpp>

#include <event/door.hpp>

#include <component/room.hpp>

#include <entt/entt.hpp>

namespace pd::listener::room
{
	namespace cr = component::room;
	namespace er = event::room;

	auto on_leave(entt::registry& registry, [[maybe_unused]] const event::room::Leave& leave) noexcept -> void
	{
		// 离开房间时有什么值得做的吗?
		// 理论上房间应该已经被清理,需要做什么检查吗?
	}

	auto on_enter(entt::registry& registry, [[maybe_unused]] const event::room::Enter& enter) noexcept -> void
	{
		using namespace component;
		using manager::Event;
		namespace ed = event::door;

		// TODO: 检查当前房间是否已清理
		// 已清理 --> 不关门
		// 未清理 --> 关门
		Event::enqueue(ed::RequestClose{});
	}
}
