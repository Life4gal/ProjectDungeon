// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/move_behavior.hpp>

#include <numbers>

#include <utility/physics.hpp>

#include <manager/random.hpp>

#include <component/enemy.hpp>

#include <accessor/collision.hpp>
#include <accessor/player_controller.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;
	namespace mbw = move_behavior::wander;
	namespace mbp = move_behavior::patrol;
	namespace mbc = move_behavior::chase;
	namespace mbj = move_behavior::jump;
	namespace mbcj = move_behavior::chase_jump;
	namespace mbt = move_behavior::teleport;

	namespace
	{
		// 是否被卡住
		[[nodiscard]] auto is_stuck(const b2Vec2 current_velocity, const float max_speed) noexcept -> bool
		{
			constexpr auto threshold = 0.1f;

			const auto p = utility::Physics::to_physics(max_speed);
			const auto p2 = p * p;

			const auto v = b2LengthSquared(current_velocity);

			return v < p2 * threshold;
		}

		namespace wander {}
	}

	auto move_behavior(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		// =========================================
		// WANDER
		// =========================================

		{
			const auto view = registry
					.view<
						const mbw::Config,
						state::InCameraArea,
						mbw::Direction,
						mbw::DirectionTimer,
						const collision::BodyId //
					>(entt::exclude<state::DisableAi>);

			for (const auto [entity, config, direction, direction_timer, body_id]: view.each())
			{
				if (direction_timer.remaining > sf::Time::Zero)
				{
					if (const auto velocity = accessor::Collision::get_linear_velocity(body_id.body_id);
						is_stuck(velocity, config.speed))
					{
						direction_timer.remaining = sf::Time::Zero;
					}
				}

				direction_timer.remaining -= delta;
				if (direction_timer.remaining < sf::Time::Zero)
				{
					const auto next_angle = manager::Random::real_inclusive(0.f, std::numbers::pi_v<float> * 2);
					const auto next_timer = manager::Random::real_inclusive(config.next_turn_min_time, config.next_turn_max_timer);

					direction.angle = sf::radians(next_angle);
					direction_timer.remaining = sf::seconds(next_timer);
				}

				const auto d = b2Vec2{.x = std::cos(direction.angle.asDegrees()), .y = std::sin(direction.angle.asDegrees())};
				const auto v = d * utility::Physics::to_physics(config.speed);
				accessor::Collision::set_linear_velocity(body_id.body_id, v);
			}
		}

		// =========================================
		// PATROL
		// =========================================

		{
			//
		}

		// =========================================
		// CHASE
		// =========================================

		{
			const auto view = registry.view<
				const mbc::Config,
				state::InCameraArea,
				const transform::Position,
				const collision::BodyId //
			>(entt::exclude<state::DisableAi>);

			for (const auto [entity, config, position, body_id]: view.each())
			{
				const auto physics_speed = utility::Physics::to_physics(config.speed);

				const auto player_position = accessor::PlayerController::get_position(registry);
				const auto direction = player_position - position.position;

				if (direction == sf::Vector2f{0, 0})
				{
					return;
				}

				const auto direction_normalized = direction.normalized();

				if (const auto velocity = accessor::Collision::get_linear_velocity(body_id.body_id);
					is_stuck(velocity, config.speed))
				{
					// 尝试垂直方向滑行
					const auto new_velocity = b2Vec2{.x = -direction_normalized.y, .y = direction_normalized.x} * physics_speed;

					accessor::Collision::set_linear_velocity(body_id.body_id, new_velocity);
				}
				else
				{
					const auto new_velocity = b2Vec2{.x = direction_normalized.x, .y = direction_normalized.y} * physics_speed;

					accessor::Collision::set_linear_velocity(body_id.body_id, new_velocity);
				}
			}
		}

		// =========================================
		// JUMP
		// =========================================

		{
			//
		}

		// =========================================
		// CHASE JUMP
		// =========================================

		{
			const auto view = registry.view<
				const mbcj::Config,
				state::InCameraArea,
				mbcj::State,
				mbcj::AirTimer,
				const transform::Position,
				const collision::BodyId //
			>(entt::exclude<state::DisableAi>);

			for (const auto [entity, config, state, air_timer, position, body_id]: view.each())
			{
				const auto physics_jump_speed = utility::Physics::to_physics(config.speed);

				if (state == mbcj::State::IDLE)
				{
					air_timer.remaining -= delta;
					if (air_timer.remaining <= sf::Time::Zero)
					{
						const auto player_position = accessor::PlayerController::get_position(registry);
						const auto direction = player_position - position.position;

						if (direction == sf::Vector2f{0, 0})
						{
							return;
						}

						state = mbcj::State::JUMPING;
						air_timer.remaining = sf::seconds(config.duration);

						const auto direction_normalized = direction.normalized();
						const auto velocity = direction_normalized * physics_jump_speed;

						accessor::Collision::set_linear_velocity(body_id.body_id, {.x = velocity.x, .y = velocity.y});
					}
				}
				else if (state == mbcj::State::JUMPING)
				{
					const auto velocity = accessor::Collision::get_linear_velocity(body_id.body_id);
					const auto speed_squared = b2LengthSquared(velocity);

					if (speed_squared > 0.001f)
					{
						const auto scale = physics_jump_speed / std::sqrt(speed_squared);

						const auto target_velocity = velocity * scale;
						accessor::Collision::set_linear_velocity(body_id.body_id, target_velocity);
					}

					air_timer.remaining -= delta;
					if (air_timer.remaining < sf::Time::Zero)
					{
						const auto next_jump_delay = manager::Random::real_inclusive(config.next_jump_min_time, config.next_jump_max_time);

						state = mbcj::State::IDLE;
						air_timer.remaining = sf::seconds(next_jump_delay);

						accessor::Collision::set_linear_velocity(body_id.body_id, b2Vec2_zero);
					}
				}
				else
				{
					PROMETHEUS_PLATFORM_UNREACHABLE();
				}
			}
		}

		// =========================================
		// TELEPORT
		// =========================================

		{
			//
		}
	}
}
