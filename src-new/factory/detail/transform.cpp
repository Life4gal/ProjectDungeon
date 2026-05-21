// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/transform.hpp>

#include <component/transform.hpp>

#include <entt/entt.hpp>

namespace pd::factory::detail
{
	using namespace component;

	auto attach(
		entt::registry& registry,
		const entt::entity entity,
		const blueprint::Position position,
		[[maybe_unused]] const blueprint::Scale scale,
		const blueprint::Rotation rotation
	) noexcept -> void
	{
		registry.emplace<transform::Position>(entity, sf::Vector2f{position.x, position.y});
		// TODO: 对于有物理刚体组件且会移动的实体,屏幕坐标后续会自动同步,对于其他实体似乎没有好的更新方式?
		registry.emplace<transform::ScreenPosition>(entity, sf::Vector2i{0, 0});

		// registry.emplace<transform::Scale>(entity, sf::Vector2f{scale.x, scale.y});

		registry.emplace<transform::Rotation>(entity, sf::degrees(rotation.rotation));
	}
}
