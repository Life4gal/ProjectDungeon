// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/commit_render_queue.hpp>

#include <component/renderer.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace pd::render
{
	using namespace component;

	namespace
	{
		sf::RenderStates g_render_states{};
	}

	auto commit_render_queue(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		// 前端BUG,移动到for里面编译器报错
		// ReSharper disable once CppTooWideScopeInitStatement
		const auto& [queue] = registry.ctx().get<const renderer::RenderCommandQueue>();
		for (const auto& [texture, vertices]: queue)
		{
			g_render_states.texture = texture.operator->();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, g_render_states);
		}
	}
}
