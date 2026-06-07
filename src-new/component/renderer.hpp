// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <unordered_map>

#include <manager/resource_fwd.hpp>

#include <blueprint/def.hpp>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace pd::component::renderer
{
	// 渲染单元集
	//
	// [CTX]
	class RenderItemSet final
	{
	public:
		class Item final
		{
		public:
			// ====================
			// transform
			// ====================

			sf::Vector2f position;
			sf::Vector2f scale;
			sf::Angle rotation;

			// ====================
			// texture
			// ====================

			blueprint::RenderLayer render_layer;
			manager::texture_handler texture;
			sf::Vector2f uv_position;
			sf::Vector2f uv_size;
			sf::Vector2f pivot;
			sf::Color color;
		};

		std::unordered_map<entt::entity, Item> set;
	};

	// 渲染命令队列
	//
	// [CTX]
	class RenderCommandQueue final
	{
	public:
		class Batch final
		{
		public:
			manager::texture_handler texture;
			std::vector<sf::Vertex> vertices;
		};

		std::vector<Batch> queue;
	};
}
