// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/sprite_effect.hpp>

#include <numbers>

#include <component/sprite_effect.hpp>
#include <component/sprite.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	namespace
	{
		// namespace position
		// {
		// 	namespace sep = sprite_effect::position;
		//
		// 	auto translate(entt::registry& registry, const sf::Time delta) noexcept -> void
		// 	{
		// 		for (const auto view = registry.view<sep::Translate, sprite::RenderPositionOffset>();
		// 		     const auto [entity,translate, render_position_offset]: view.each())
		// 		{
		// 			translate.elapsed += delta;
		// 			if (translate.elapsed >= translate.duration)
		// 			{
		// 				registry.remove<sep::Translate>(entity);
		// 				continue;
		// 			}
		//
		// 			const auto ratio = translate.elapsed / translate.duration;
		// 			const auto diff = translate.to - translate.from;
		//
		// 			render_position_offset.offset =
		// 					// 线性插值
		// 					translate.from + diff * ratio;
		// 		}
		// 	}
		//
		// 	auto update(entt::registry& registry, const sf::Time delta) noexcept -> void
		// 	{
		// 		// TRANSLATE
		// 		translate(registry, delta);
		// 	}
		// }

		namespace scale
		{
			namespace ses = sprite_effect::scale;

			auto linear(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				for (const auto view = registry.view<ses::Linear, sprite::RenderScale>();
				     const auto [entity, linear, render_scale]: view.each())
				{
					linear.elapsed += delta;
					if (linear.elapsed >= linear.duration)
					{
						registry.remove<ses::Linear>(entity);
						continue;
					}

					const auto ratio = linear.elapsed / linear.duration;
					const auto diff = linear.to - linear.from;

					render_scale.scale =
							// 线性插值
							linear.from + diff * ratio;
				}
			}

			auto shockwave(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				for (const auto view = registry.view<ses::Shockwave, sprite::RenderScale>();
				     const auto [entity, shockwave, render_scale]: view.each())
				{
					shockwave.elapsed += delta;
					if (shockwave.elapsed >= shockwave.duration)
					{
						registry.remove<ses::Shockwave>(entity);
						continue;
					}

					const auto ratio = shockwave.elapsed / shockwave.duration;
					// 使用正弦函数模拟冲击波
					const auto pulse = std::sinf(ratio * std::numbers::pi_v<float>);
					const auto scale = 1.f + (shockwave.peak - 1.f) * pulse;

					render_scale.scale = shockwave.base * scale;
				}
			}

			auto bounce(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				for (const auto view = registry.view<ses::Bounce, sprite::RenderScale>();
				     const auto [entity, bounce, render_scale]: view.each())
				{
					bounce.elapsed += delta;
					if (bounce.elapsed >= bounce.duration)
					{
						registry.remove<ses::Bounce>(entity);
						continue;
					}

					const auto elapsed_seconds = bounce.elapsed.asSeconds();
					const auto decay = std::expf(-elapsed_seconds * bounce.stiffness);
					const auto oscillation = decay * std::cosf(elapsed_seconds * std::numbers::pi_v<float> * 2 * bounce.stiffness);
					const auto diff = bounce.to - bounce.from;

					render_scale.scale = bounce.to - diff * oscillation;
				}
			}

			auto breathing(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				for (const auto view = registry.view<ses::Breathing, sprite::RenderScale>();
				     const auto [entity, breathing, render_scale]: view.each())
				{
					breathing.elapsed += delta;
					if (breathing.elapsed >= breathing.duration)
					{
						registry.remove<ses::Breathing>(entity);
						continue;
					}

					const auto elapsed_seconds = breathing.elapsed.asSeconds();
					const auto factor = elapsed_seconds * breathing.speed;
					const auto pulse = (std::sinf(factor) + 1) * 0.5f;
					const auto diff = breathing.max - breathing.min;
					const auto scale = breathing.min + diff * pulse;

					render_scale.scale = breathing.base * scale;
				}
			}

			auto update(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				// LINEAR
				linear(registry, delta);
				// SHOCKWAVE
				shockwave(registry, delta);
				// BOUNCE
				bounce(registry, delta);
				// BREATHING
				breathing(registry, delta);
			}
		}

		namespace color
		{
			namespace sec = sprite_effect::color;

			auto linear(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				for (const auto view = registry.view<sec::Linear, sprite::RenderColor>();
				     const auto [entity, linear, render_color]: view.each())
				{
					linear.elapsed += delta;
					if (linear.elapsed >= linear.duration)
					{
						registry.remove<sec::Linear>(entity);
						continue;
					}

					const auto ratio = linear.elapsed / linear.duration;
					const auto diff = linear.to - linear.from;

					render_color.color =
							// 线性插值
							linear.from +
							sf::Color
							{
									static_cast<std::uint8_t>(static_cast<float>(diff.r) * ratio),
									static_cast<std::uint8_t>(static_cast<float>(diff.g) * ratio),
									static_cast<std::uint8_t>(static_cast<float>(diff.b) * ratio),
									static_cast<std::uint8_t>(static_cast<float>(diff.a) * ratio),
							};
				}
			}

			auto update(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				// LINEAR
				linear(registry, delta);
			}
		}
	}

	auto sprite_effect(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		// FIXME: 精灵动画特效更新没有考虑动画暂停

		// =========================================
		// POSITION
		// =========================================

		// position::update(registry, delta);

		// =========================================
		// SCALE
		// =========================================

		scale::update(registry, delta);

		// =========================================
		// COLOR
		// =========================================

		color::update(registry, delta);
	}
}
