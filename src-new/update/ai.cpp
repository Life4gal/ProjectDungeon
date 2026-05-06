// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/ai.hpp>

#include <numbers>

#include <utility/physics.hpp>

#include <manager/random.hpp>

#include <component/enemy.hpp>
#include <component/player.hpp>

// get_player_position
#include <component/player_controller.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::update
{
	using namespace component;

	namespace
	{
		// TODO: 把这个接口移动到合理的位置
		[[nodiscard]] auto get_player_position(entt::registry& registry) noexcept -> sf::Vector2f
		{
			const auto* target = registry.ctx().find<player_controller::Target>();
			if (target == nullptr or not registry.valid(target->entity))
			{
				return {0, 0};
			}

			const auto& [position] = registry.get<const position::World>(target->entity);
			return position;
		}

		[[nodiscard]] auto is_wall_stuck(const b2Vec2 velocity, const actor::Speed& max_speed) noexcept -> bool
		{
			constexpr auto wall_stuck_threshold = 0.1f;

			const auto physics_max_speed = utility::Physics::to_physics(max_speed.speed);
			const auto physics_max_speed_squared = physics_max_speed * physics_max_speed;

			const auto speed_squared = b2LengthSquared(velocity);

			return speed_squared < physics_max_speed_squared * wall_stuck_threshold;
		}

		namespace wander
		{
			namespace aw = ai::wander;

			constexpr auto WanderTimeMin = 1.5f;
			constexpr auto WanderTimeMax = 4.f;

			auto update(
				const sf::Time delta,
				aw::Direction& direction,
				aw::DirectionTimer& direction_timer,
				const actor::Speed max_speed,
				const physics_body::Id body_id
			) noexcept -> void
			{
				if (direction_timer.remaining > sf::Time::Zero)
				{
					if (const auto velocity = b2Body_GetLinearVelocity(body_id.id);
						is_wall_stuck(velocity, max_speed))
					{
						direction_timer.remaining = sf::Time::Zero;
					}
				}

				direction_timer.remaining -= delta;
				if (direction_timer.remaining < sf::Time::Zero)
				{
					const auto next_angle = manager::Random::real_inclusive(0.f, std::numbers::pi_v<float> * 2);
					const auto next_timer = manager::Random::real_inclusive(WanderTimeMin, WanderTimeMax);

					direction.angle = sf::radians(next_angle);
					direction_timer.remaining = sf::seconds(next_timer);
				}

				const auto d = b2Vec2{.x = std::cos(direction.angle.asDegrees()), .y = std::sin(direction.angle.asDegrees())};
				const auto v = d * utility::Physics::to_physics(max_speed.speed);
				b2Body_SetLinearVelocity(body_id.id, v);
			}
		}

		namespace jump
		{
			namespace aj = ai::jump;

			constexpr auto JumSpeed = 250.f;
			constexpr auto JumpDuration = 0.6f;
			constexpr auto JumpLandDelayMin = 0.8f;
			constexpr auto JumpLandDelayMax = 1.6f;

			auto update(
				entt::registry& registry,
				const sf::Time delta,
				aj::State& state,
				aj::AirTimer& air_timer,
				const position::World position,
				const physics_body::Id body_id
			) noexcept -> void
			{
				constexpr auto physics_jump_speed = utility::Physics::to_physics(JumSpeed);

				if (state == aj::State::IDLE)
				{
					air_timer.remaining -= delta;
					if (air_timer.remaining <= sf::Time::Zero)
					{
						const auto player_position = get_player_position(registry);
						const auto direction = player_position - position.position;

						if (direction == sf::Vector2f{0, 0})
						{
							return;
						}

						state = aj::State::JUMPING;
						air_timer.remaining = sf::seconds(JumpDuration);

						const auto direction_normalized = direction.normalized();
						const auto velocity = direction_normalized * physics_jump_speed;

						b2Body_SetLinearVelocity(body_id.id, {.x = velocity.x, .y = velocity.y});
					}
				}
				else if (state == aj::State::JUMPING)
				{
					const auto velocity = b2Body_GetLinearVelocity(body_id.id);
					const auto speed_squared = b2LengthSquared(velocity);

					if (speed_squared > 0.001f)
					{
						const auto scale = physics_jump_speed / std::sqrt(speed_squared);

						const auto target_velocity = velocity * scale;
						b2Body_SetLinearVelocity(body_id.id, target_velocity);
					}

					air_timer.remaining -= delta;
					if (air_timer.remaining < sf::Time::Zero)
					{
						const auto next_jump_delay = manager::Random::real_inclusive(JumpLandDelayMin, JumpLandDelayMax);

						state = aj::State::IDLE;
						air_timer.remaining = sf::seconds(next_jump_delay);

						b2Body_SetLinearVelocity(body_id.id, b2Vec2_zero);
					}
				}
				else
				{
					PROMETHEUS_PLATFORM_UNREACHABLE();
				}
			}
		}

		namespace chase
		{
			// namespace ac = ai::chase;

			auto update(
				entt::registry& registry,
				const actor::Speed& max_speed,
				const position::World position,
				const physics_body::Id body_id
			) noexcept -> void
			{
				const auto player_position = get_player_position(registry);
				const auto direction = player_position - position.position;

				if (direction == sf::Vector2f{0, 0})
				{
					return;
				}

				const auto direction_normalized = direction.normalized();

				if (const auto velocity = b2Body_GetLinearVelocity(body_id.id);
					is_wall_stuck(velocity, max_speed))
				{
					// 尝试垂直方向滑行
					const auto new_velocity = b2Vec2{.x = -direction_normalized.y, .y = direction_normalized.x} * utility::Physics::to_physics(max_speed.speed);

					b2Body_SetLinearVelocity(body_id.id, new_velocity);
				}
				else
				{
					const auto new_velocity = b2Vec2{.x = direction_normalized.x, .y = direction_normalized.y} * utility::Physics::to_physics(max_speed.speed);

					b2Body_SetLinearVelocity(body_id.id, new_velocity);
				}
			}
		}

		//
	}

	auto ai(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		// =========================================
		// WANDER
		// =========================================

		{
			const auto view = registry
					.view<
						state::InCameraArea,
						ai::wander::Direction,
						ai::wander::DirectionTimer,
						const actor::Speed,
						const physics_body::Id //
					>(entt::exclude<state::Dead>);

			for (const auto [entity, direction, direction_timer, speed, body_id]: view.each())
			{
				wander::update(delta, direction, direction_timer, speed, body_id);
			}
		}

		// =========================================
		// JUMP
		// =========================================

		{
			const auto view = registry
					.view<
						state::InCameraArea,
						ai::jump::State,
						ai::jump::AirTimer,
						const position::World,
						const physics_body::Id //
					>(entt::exclude<state::Dead>);

			for (const auto [entity, state, air_timer, position, body_id]: view.each())
			{
				jump::update(registry, delta, state, air_timer, position, body_id);
			}
		}

		// =========================================
		// CHASE
		// =========================================

		{
			const auto view = registry
					.view<
						state::InCameraArea,
						ai::chase::Placeholder,
						const actor::Speed,
						const position::World,
						const physics_body::Id //
					>(entt::exclude<state::Dead>);

			for (const auto [entity, speed, position, body_id]: view.each())
			{
				chase::update(registry, speed, position, body_id);
			}
		}

		// =========================================
		// 
		// =========================================
	}
}
