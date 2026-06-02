// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/room_guardian.hpp>

#include <component/level.hpp>
#include <component/tags.hpp>
#include <component/transform.hpp>

#include <helper/camera.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	// FIXME: 与update::new_entity_tag中存在几乎相同代码
	auto room_guardian(entt::registry& registry, [[maybe_unused]] sf::Time delta) noexcept -> void
	{
		if (not registry.ctx().contains<level::RoomChanged>())
		{
			return;
		}
		registry.ctx().erase<level::RoomChanged>();

		const auto camera_area = helper::Camera::get_area(registry);

		for (const auto view = registry.view<transform::Position>();
		     const auto [entity, position]: view.each())
		{
			// FIXME: 不能简单地用position筛选处于相机范围内的实体,还需要考虑实体的size,当且仅当实体的bounding box完全不与相机范围相交时才认为实体不在相机范围内
			// 考虑到我们的实现,似乎并不需要考虑实体的size,因为我们在生成实体时就会将其position设置为实体的bounding box的中心点,因此只要position在相机范围内,就认为该实体在相机范围内
			if (const auto in_area = camera_area.contains(position.position);
				in_area)
			{
				registry.emplace<state::InCameraArea>(entity);
			}
			else
			{
				registry.remove<state::InCameraArea>(entity);
			}
		}
	}
}
