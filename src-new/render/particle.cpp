// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/particle.hpp>

#include <manager/resource.hpp>

#include <component/particle.hpp>

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

	auto particle(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		namespace rss = component::render::static_sprite;

		const auto view = registry.view<
			state::InCameraArea,
			rss::Texture,
			rss::UvPosition,
			rss::UvSize,
			rss::Pivot,
			particle::Shader,
			limited_life::time::Lifetime,
			limited_life::time::Elapsed,
			particle::ShaderParams,
			transform::Position,
			transform::Rotation
		>();

		for (const auto [
			     entity,
			     texture,
			     texture_position,
			     texture_size,
			     texture_pivot,
			     shader,
			     lifetime,
			     elapsed,
			     shader_params,
			     position,
			     rotation
		     ]: view.each())
		{
			const auto render_position = position.position;
			constexpr auto render_color = sf::Color::White;
			const auto render_pivot = texture_pivot.pivot;

			if (texture.texture == manager::InvalidHandler or shader.shader == manager::InvalidHandler)
			{
				SPDLOG_WARN(
					"无法渲染位于({:.1f}:{:.1f})的粒子(0x{:08x}),无效的纹理[{}]或着色器[{}]资源句柄!",
					render_position.x,
					render_position.y,
					static_cast<std::uint32_t>(entity),
					manager::Texture::path_of(texture.texture),
					manager::Shader::path_of(shader.shader)
				);

				continue;
			}

			const auto t = elapsed.elapsed / lifetime.lifetime;
			const auto scale = shader_params.start_scale + (shader_params.end_scale - shader_params.start_scale) * t;

			// 变换矩阵
			const auto model = sf::Transform{} //
			                   .translate(render_position)
			                   .scale({scale, scale})
			                   .translate(-render_pivot);
			// mvp
			const auto mvp = window.getView().getTransform() * model;

			shader.shader->setUniform("u_mvp", sf::Glsl::Mat4{mvp});
			shader.shader->setUniform("u_texture", *texture.texture);
			shader.shader->setUniform("u_start_color", sf::Glsl::Vec4{shader_params.start_color});
			shader.shader->setUniform("u_end_color", sf::Glsl::Vec4{shader_params.end_color});
			shader.shader->setUniform("u_ratio", t);

			const auto vertices = [&] noexcept -> auto
			{
				const auto texture_width = texture_size.size.x;
				const auto texture_height = texture_size.size.y;

				const auto texture_left = texture_position.position.x;
				const auto texture_top = texture_position.position.y;
				const auto texture_right = texture_left + texture_size.size.x;
				const auto texture_bottom = texture_top + texture_size.size.y;

				// 归一化uv
				// 除以整张纹理大小,而不是该子纹理大小
				const auto full_texture_size = sf::Vector2f{texture.texture->getSize()};
				const auto coordinate_left = texture_left / full_texture_size.x;
				const auto coordinate_top = texture_top / full_texture_size.y;
				const auto coordinate_right = texture_right / full_texture_size.x;
				const auto coordinate_bottom = texture_bottom / full_texture_size.y;

				return std::array<sf::Vertex, 4>
				{{
						{.position = {0, 0}, .color = render_color, .texCoords = {coordinate_left, coordinate_top}},
						{.position = {texture_width, 0}, .color = render_color, .texCoords = {coordinate_right, coordinate_top}},
						{.position = {0, texture_height}, .color = render_color, .texCoords = {coordinate_left, coordinate_bottom}},
						{.position = {texture_width, texture_height}, .color = render_color, .texCoords = {coordinate_right, coordinate_bottom}},
				}};
			}();

			g_shared_states.shader = shader.shader.operator->();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, g_shared_states);
		}
	}
}
