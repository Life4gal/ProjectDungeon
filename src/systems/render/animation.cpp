// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/render/animation.hpp>

#include <ctx/asset_loader.hpp>

#include <components/transform.hpp>
#include <components/animation.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace pd::systems::render
{
	auto animation(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		using namespace components;

		// 当前共用RenderStates
		static sf::RenderStates states{};

		for (const auto view = registry.view<
			     const Position,
			     const Scale,
			     const Rotation,
			     const AnimationIndex,
			     const Animation>();
		     const auto [entity, position, scale, rotation, animation_index, animation]: view.each())
		{
			const auto& frame = animation.frames[animation_index.current];
			const auto texture = ctx::TextureLoader::get(registry, frame.texture);

			if (not texture)
			{
				// todo: 默认纹理?
				SPDLOG_ERROR(
					"无法渲染位于({:.3f}:{:.3f})的实体[{}],纹理ID: {}",
					position.position.x,
					position.position.y,
					entt::to_integral(entity),
					frame.texture
				);
				continue;
			}

			const auto x = static_cast<float>(frame.frame_x);
			const auto y = static_cast<float>(frame.frame_y);
			const auto width = static_cast<float>(frame.frame_width);
			const auto height = static_cast<float>(frame.frame_height);
			// todo: 原点一定是中心点吗?
			const auto origin = sf::Vector2f{width, height} / 2.f;

			const auto vertices = [&] noexcept -> auto
			{
				// todo: 能不能控制颜色?
				constexpr auto color = sf::Color::White;

				const auto right = x + width;
				const auto bottom = y + height;

				return std::array<sf::Vertex, 4>
				{{
						{.position = {0, 0}, .color = color, .texCoords = {x, y}},
						{.position = {width, 0}, .color = color, .texCoords = {right, y}},
						{.position = {0, height}, .color = color, .texCoords = {x, bottom}},
						{.position = {width, height}, .color = color, .texCoords = {right, bottom}},
				}};
			}();

			states.texture = texture.operator->();
			states.transform = [&] noexcept -> sf::Transform
			{
				sf::Transform t{};
				t.translate(position.position);
				t.scale(scale.scale);
				t.rotate(rotation.rotation);
				t.translate(-origin);

				return t;
			}();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, states);
		}
	}
}
