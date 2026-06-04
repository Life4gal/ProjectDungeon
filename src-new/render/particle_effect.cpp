// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/particle_effect.hpp>

#include <component/particle_effect.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace pd::render
{
	using namespace component;

	namespace
	{
		sf::RenderStates g_shared_states{};
	}

	auto particle_effect(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		const auto view = registry
				.view<
					const particle_effect::RenderLayers,
					const particle_effect::ParticleLifetime,
					const particle_effect::Particles //
				>();

		for (const auto [entity, layers, particle_lifetime, particles]: view.each())
		{
			for (const auto& particle: particles.particles)
			{
				const auto t = particle.elapsed / particle_lifetime.lifetime;

				for (const auto& layer: layers.layers)
				{
					// 设置着色器参数
					layer.shader->setUniform("u_texture", sf::Shader::CurrentTexture);
					layer.shader->setUniform("u_start_color", sf::Glsl::Vec4{layer.start_color});
					layer.shader->setUniform("u_end_color", sf::Glsl::Vec4{layer.end_color});
					layer.shader->setUniform("u_ratio", t);

					const auto render_positon = particle.position;
					const auto render_scale = std::lerp(layer.start_scale, layer.end_scale, t);
					constexpr auto render_rotation = sf::radians(0);
					constexpr auto render_color = sf::Color::White;
					const auto render_pivot = layer.pivot;

					const auto vertices = [&] noexcept -> auto
					{
						const auto texture_width = layer.uv_size.x;
						const auto texture_height = layer.uv_size.y;

						const auto texture_left = layer.uv_position.x;
						const auto texture_top = layer.uv_position.y;
						const auto texture_right = texture_left + layer.uv_size.x;
						const auto texture_bottom = texture_top + layer.uv_size.y;

						return std::array<sf::Vertex, 4>
						{{
								{.position = {0, 0}, .color = render_color, .texCoords = {texture_left, texture_top}},
								{.position = {texture_width, 0}, .color = render_color, .texCoords = {texture_right, texture_top}},
								{.position = {0, texture_height}, .color = render_color, .texCoords = {texture_left, texture_bottom}},
								{.position = {texture_width, texture_height}, .color = render_color, .texCoords = {texture_right, texture_bottom}},
						}};
					}();

					g_shared_states.texture = layer.texture.operator->();
					g_shared_states.blendMode = layer.blend_mode;
					g_shared_states.shader = layer.shader.operator->();
					g_shared_states.transform = [&] noexcept -> sf::Transform
					{
						return sf::Transform{} //
						       .translate(render_positon)
						       .rotate(render_rotation)
						       .scale({render_scale, render_scale})
						       .translate(-render_pivot);
					}();

					window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, g_shared_states);
				}
			}
		}
	}
}
