// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/render.hpp>

#include <config/dungeon.hpp>

#include <components/render.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto Render::attach(
		entt::registry& registry,
		const entt::entity entity,
		const config::AnimationFrame& animation_frame,
		config::RenderLayer render_layer,
		const sf::Color color
	) noexcept -> void
	{
		using namespace components;

		registry.emplace_or_replace<render::AnimationFrame>(entity, std::cref(animation_frame));
		registry.emplace_or_replace<render::RenderLayer>(entity, render_layer);
		registry.emplace_or_replace<render::Color>(entity, color);

		registry.ctx().emplace<render::SortRequired>();
	}

	auto Render::deattach(entt::registry& registry, const entt::entity entity_with_render) noexcept -> void
	{
		using namespace components;

		registry.remove<render::AnimationFrame>(entity_with_render);
		registry.remove<render::RenderLayer>(entity_with_render);
		registry.remove<render::Color>(entity_with_render);
	}

	auto Render::hide(entt::registry& registry, const entt::entity entity_with_render) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<render::AnimationFrame>(entity_with_render));

		registry.emplace_or_replace<render::Invisible>(entity_with_render);
	}

	auto Render::show(entt::registry& registry, const entt::entity entity_with_render) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<render::AnimationFrame>(entity_with_render));

		registry.remove<render::Invisible>(entity_with_render);
	}

	auto Render::get_animation_frame(const entt::registry& registry, const entt::entity entity_with_render) noexcept -> const config::AnimationFrame&
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<render::AnimationFrame>(entity_with_render));

		return registry.get<const render::AnimationFrame>(entity_with_render).animation_frame;
	}

	auto Render::get_layer(const entt::registry& registry, const entt::entity entity_with_render) noexcept -> config::RenderLayer
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<render::AnimationFrame, render::RenderLayer>(entity_with_render)));

		return registry.get<const render::RenderLayer>(entity_with_render).render_layer;
	}

	auto Render::change_layer(entt::registry& registry, const entt::entity entity_with_render, const config::RenderLayer new_layer) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<render::AnimationFrame>(entity_with_render));

		registry.emplace_or_replace<render::RenderLayer>(entity_with_render, new_layer);

		registry.ctx().emplace<render::SortRequired>();
	}

	auto Render::get_color(const entt::registry& registry, const entt::entity entity_with_render) noexcept -> sf::Color
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<render::Color>(entity_with_render));

		return registry.get<const render::Color>(entity_with_render).color;
	}

	auto Render::change_color(entt::registry& registry, const entt::entity entity_with_render, sf::Color new_color) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<render::Color>(entity_with_render));

		registry.emplace_or_replace<render::Color>(entity_with_render, new_color);
	}
}
