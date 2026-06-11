// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/animation.hpp>

#include <component/animation.hpp>
#include <component/render.hpp>
#include <component/collision.hpp>
#include <component/tags.hpp>

#include <accessor/collision.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::update
{
	using namespace component;

	namespace
	{
		[[nodiscard]] constexpr auto velocity_to_facing(const b2Vec2 velocity) noexcept -> animation::Direction
		{
			const auto x = std::abs(velocity.x);
			const auto y = std::abs(velocity.y);

			if (x >= y)
			{
				if (velocity.x >= 0)
				{
					return blueprint::Direction::EAST;
				}

				return blueprint::Direction::WEST;
			}

			if (velocity.y >= 0)
			{
				return blueprint::Direction::SOUTH;
			}

			return blueprint::Direction::NORTH;
		}
	}

	auto animation(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					state::InCameraArea,
					animation::Direction,
					const animation::Groups,
					render::dynamic_sprite::BeginFrameIndex,
					render::dynamic_sprite::EndFrameIndex,
					render::dynamic_sprite::CurrentFrameIndex,
					const collision::BodyId //
				>();

		for (const auto [entity, direction, groups, begin_frame_index, end_frame_index, current_frame_index, body_id]: view.each())
		{
			const auto velocity = accessor::Collision::get_linear_velocity(body_id.body_id);
			const auto facing = velocity_to_facing(velocity);

			// 方向不变
			if (facing == direction)
			{
				continue;
			}

			const auto& [begin_frame, end_frame] = groups.groups[std::to_underlying(facing)];

			direction = facing;
			begin_frame_index.index = begin_frame;
			end_frame_index.index = end_frame;
			current_frame_index.index = begin_frame;
		}
	}
}
