// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/render_effect.hpp>

#include <component/render_effect.hpp>

#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	auto RenderEffect::make(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		namespace res = render_effect::sprite;

		registry.emplace<res::Position>(entity, sf::Vector2f{0, 0});
		registry.emplace<res::Scale>(entity, sf::Vector2f{1, 1});
		registry.emplace<res::Rotation>(entity, sf::degrees(0));
		registry.emplace<res::Color>(entity, sf::Color::White);
	}
}
