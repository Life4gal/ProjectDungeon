// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/render_effect.hpp>

#include <numbers>

#include <manager/random.hpp>

#include <component/render_effect.hpp>

#include <prometheus/functional/functor.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	namespace
	{
		namespace position
		{
			namespace rep = render_effect::position;

			auto update(entt::registry& registry, const float delta) noexcept -> void
			{
				const auto visitor = prometheus::functional::overloaded
				{
						[&](const entt::entity entity, render_effect::Position& position, rep::Linear& linear) noexcept -> void
						{
							// 此帧移动距离
							const auto traveled = linear.speed * delta;

							linear.traveled += traveled;
							if (linear.traveled >= linear.distance)
							{
								// 移除特效
								registry.remove<rep::Linear>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto ratio = linear.traveled / linear.distance;
							const auto offset = linear.offset * ratio;

							position.extra = linear.start + offset;
						},
						[&](const entt::entity entity, render_effect::Position& position, rep::Oscillator& oscillator) noexcept -> void
						{
							oscillator.elapsed += delta;
							if (oscillator.elapsed >= oscillator.duration)
							{
								// 移除特效
								registry.remove<rep::Oscillator>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							// 已移动距离
							const auto traveled = oscillator.speed * oscillator.elapsed;
							// 取模以得到当前往返周期中的移动距离
							const auto mod = std::fmod(traveled, oscillator.distance * 2);

							// 转换到 [0, distance] 区间(三角波)
							auto t = mod;
							if (t > oscillator.distance)
							{
								t = oscillator.distance * 2 - t;
							}

							const auto ratio = t / oscillator.distance;
							const auto offset = oscillator.offset * ratio;

							position.extra = oscillator.p1 + offset;
						},
						[&](const entt::entity entity, render_effect::Position& position, rep::Spring& spring) noexcept -> void
						{
							spring.elapsed += delta;
							if (spring.elapsed >= spring.duration)
							{
								// 移除特效
								registry.remove<rep::Spring>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto displacement = spring.position - spring.target;

							// 弹簧力: F = -k * x
							auto f = -spring.strength * displacement;
							// 阻尼力
							f -= spring.damping * spring.velocity;
							// 欧拉积分
							spring.velocity += f * delta;
							spring.position += spring.velocity * delta;

							position.extra = spring.position;
						},
						[&](const entt::entity entity, render_effect::Position& position, rep::Path& path) noexcept -> void
						{
							const auto segment_count = path.waypoints.size() - 1;

							path.elapsed += delta;
							if (path.elapsed >= path.duration or path.current_segment == segment_count)
							{
								// 移除特效
								registry.remove<rep::Path>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							// 此帧移动距离
							const auto traveled = path.speed * delta;
							path.current_segment_traveled += traveled;

							if (path.current_segment_traveled >= path.distances[path.current_segment])
							{
								// 进入下一段路径
								path.current_segment += 1;
								path.current_segment_traveled = 0;
							}

							const auto this_segment_start = path.waypoints[path.current_segment];
							const auto this_segment_offset = path.offsets[path.current_segment];
							const auto this_segment_distance = path.distances[path.current_segment];

							const auto ratio = path.current_segment_traveled / this_segment_distance;
							const auto offset = this_segment_offset * ratio;
							path.position = this_segment_start + offset;

							position.extra = path.position;
						},
						[&](const entt::entity entity, render_effect::Position& position, rep::Orbit& orbit) noexcept -> void
						{
							orbit.elapsed += delta;
							if (orbit.elapsed >= orbit.duration)
							{
								// 移除特效
								registry.remove<rep::Orbit>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							orbit.angle += orbit.speed * delta;
							const auto offset = sf::Vector2f{std::cos(orbit.angle), std::sin(orbit.angle)} * orbit.radius;

							position.extra = orbit.center + offset;
						},
						[&](const entt::entity entity, render_effect::Position& position, rep::Shake& shake) noexcept -> void
						{
							shake.elapsed += delta;
							if (shake.elapsed >= shake.duration)
							{
								// 移除特效
								registry.remove<rep::Shake>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							shake.phase += shake.frequency * delta;

							auto current_intensity = shake.intensity;
							if (shake.decay > 0)
							{
								const auto t = shake.elapsed / shake.duration;
								current_intensity = shake.intensity * (1 - t * shake.decay);
							}

							auto offset = sf::Vector2f{std::sin(shake.phase * std::numbers::pi_v<float> * 2), std::cos(shake.phase * std::numbers::pi_v<float> * 1.7f)};

							offset *= current_intensity;
							offset += sf::Vector2f{1, 1} * current_intensity * 0.5f * manager::Random::real_inclusive(-0.5f, 0.5f);

							position.extra = shake.center + offset;
						},
						[&](const entt::entity entity, render_effect::Position& position, rep::Wave& wave) noexcept -> void
						{
							wave.elapsed += delta;
							if (wave.elapsed >= wave.duration)
							{
								// 移除特效
								registry.remove<rep::Wave>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							wave.traveled += wave.speed * delta;

							// 波形偏移(正弦波)
							const auto wave_offset = wave.amplitude * std::sin(wave.traveled * std::numbers::pi_v<float> * 2 / wave.wavelength);
							// 前进方向
							const auto forward = wave.start + wave.direction * wave.traveled;
							// 垂直偏移
							const auto perpendicular_offset = wave.perpendicular_direction * wave_offset;

							position.extra = forward + perpendicular_offset;
						},
						[&](const entt::entity entity, render_effect::Position& position, rep::Swing& swing) noexcept -> void
						{
							swing.elapsed += delta;
							if (swing.elapsed >= swing.duration)
							{
								// 移除特效
								registry.remove<rep::Swing>(entity);
								// 重置位置
								position.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto sign = swing.end_angle > swing.start_angle ? 1.f : -1.f;

							const auto traveled = swing.speed * swing.elapsed;
							const auto mod = std::fmod(traveled, swing.angle_total);
							const auto angle = [&] noexcept -> float
							{
								if (mod < swing.angle_diff)
								{
									// 正向摆动
									return swing.start_angle + mod * sign;
								}

								// 反向摆动
								const auto reverse_mod = mod - swing.angle_diff;
								return swing.end_angle - reverse_mod * sign;
							}();
							const auto offset = sf::Vector2f{std::sin(angle), std::cos(angle)} * swing.radius;

							position.extra = swing.pivot + offset;
						},

						// ================================

						[&](this const auto& self, const entt::entity entity, render_effect::Position& position) noexcept -> auto
						{
							return [&self, entity, &position](auto& e) noexcept -> void // NOLINT(clang-diagnostic-padded)
							{
								self(entity, position, e);
							};
						}
				};

				for (const auto view = registry.view<rep::Effect, render_effect::Position>();
				     const auto [entity, effect, position]: view.each())
				{
					std::visit(visitor(entity, position), effect);
				}
			}
		}

		namespace scale
		{
			namespace res = render_effect::scale;

			auto update(entt::registry& registry, const float delta) noexcept -> void
			{
				const auto visitor = prometheus::functional::overloaded
				{
						[&](const entt::entity entity, render_effect::Scale& scale, res::Shrink& shrink) noexcept -> void
						{
							shrink.elapsed += delta;
							if (shrink.elapsed >= shrink.duration)
							{
								// 移除特效
								registry.remove<res::Shrink>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto t = shrink.elapsed / shrink.duration;
							const auto x = std::lerp(shrink.start_scale.x, shrink.end_scale.x, t);
							const auto y = std::lerp(shrink.start_scale.y, shrink.end_scale.y, t);

							scale.extra = {x, y};
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::Oscillator& oscillator) noexcept -> void
						{
							oscillator.elapsed += delta;
							if (oscillator.elapsed >= oscillator.duration)
							{
								// 移除特效
								registry.remove<res::Oscillator>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto traveled = oscillator.speed * oscillator.elapsed;
							const auto calculate_offset = [&]<typename T>(float T::* member) noexcept -> float
							{
								const auto swing = oscillator.swing.*member;
								const auto mod = std::fmod(traveled, swing);

								if (mod < oscillator.diff.*member)
								{
									return oscillator.min.*member + mod;
								}

								return oscillator.max.*member - (mod - oscillator.diff.*member);
							};
							const auto offset = sf::Vector2f{calculate_offset(&sf::Vector2f::x), calculate_offset(&sf::Vector2f::y)};

							scale.extra = offset;
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::Spring& spring) noexcept -> void
						{
							spring.elapsed += delta;
							if (spring.elapsed >= spring.duration)
							{
								// 移除特效
								registry.remove<res::Spring>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto displacement = spring.scale - spring.max;

							// 弹簧力: F = -k * x
							auto f = -spring.strength * displacement;
							// 阻尼力
							f -= spring.damping * spring.velocity;
							// 欧拉积分
							spring.velocity += f * delta;
							spring.scale += spring.velocity * delta;

							scale.extra = spring.scale;
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::Breathing& breathing) noexcept -> void
						{
							breathing.elapsed += delta;
							if (breathing.elapsed >= breathing.duration)
							{
								// 移除特效
								registry.remove<res::Breathing>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							// 使用正弦波模拟平滑呼吸
							const auto t = breathing.speed * breathing.elapsed * std::numbers::pi_v<float> * 2;
							const auto normalized = (std::sin(t) + 1.f) / 2.f;
							const auto offset = breathing.diff * normalized;

							scale.extra = breathing.min + offset;
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::SquashStretch& squash_stretch) noexcept -> void
						{
							squash_stretch.elapsed += delta;
							if (squash_stretch.elapsed >= squash_stretch.duration)
							{
								// 移除特效
								registry.remove<res::SquashStretch>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto factor = [&] noexcept -> float
							{
								constexpr auto phase_threshold = 0.3f;
								const auto t = squash_stretch.elapsed / squash_stretch.duration;

								// 压扁阶段(前30%)
								if (t < phase_threshold)
								{
									return 1.0f + (squash_stretch.squash_factory - 1.0f) * (t / phase_threshold);
								}

								// 恢复阶段(后70%)
								const auto restore_t = (t - phase_threshold) / (1 - phase_threshold);
								return squash_stretch.squash_factory + (1.0f - squash_stretch.squash_factory) * restore_t;
							}();

							const auto fx = 1.0f / std::sqrt(factor);
							const auto fy = std::sqrt(factor);

							// normal_scale = {1, 1}
							// scale.extra = {normal_scale.x * fx, normal_scale.y * fy};
							scale.extra = {fx, fy};
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::DirectionalPulse& directional_pulse) noexcept -> void
						{
							directional_pulse.elapsed += delta;
							if (directional_pulse.elapsed >= directional_pulse.duration)
							{
								// 移除特效
								registry.remove<res::DirectionalPulse>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							// 脉冲波形
							const auto t = directional_pulse.speed * directional_pulse.elapsed * std::numbers::pi_v<float> * 2;
							const auto pulse = std::abs(std::sin(t));

							// 构建变换矩阵的缩放因子
							const auto perpendicular_scale = 1.0f + (directional_pulse.scale.x - 1.0f) * pulse;
							const auto parallel_scale = 1.0f + (directional_pulse.scale.y - 1.0f) * pulse;

							scale.extra = {perpendicular_scale, parallel_scale};
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::Jelly& jelly) noexcept -> void
						{
							jelly.elapsed += delta;
							if (jelly.elapsed >= jelly.duration)
							{
								// 移除特效
								registry.remove<res::Jelly>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							const auto displacement = jelly.current_scale - jelly.target_scale;

							// 阻尼谐振子
							auto accumulate = -jelly.frequency * jelly.frequency * displacement;
							accumulate -= 2.0f * jelly.damping * jelly.frequency * jelly.velocity;

							jelly.velocity += accumulate * delta;
							jelly.current_scale += jelly.velocity * delta;

							scale.extra = jelly.current_scale;
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::ElasticHit& elastic_hit) noexcept -> void
						{
							using phase = res::ElasticHit::Phase;

							elastic_hit.elapsed += delta;
							if (elastic_hit.elapsed >= elastic_hit.duration)
							{
								// 移除特效
								registry.remove<res::ElasticHit>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							elastic_hit.phase_elapsed += delta;
							if (elastic_hit.phase_elapsed >= elastic_hit.phase_duration)
							{
								if (elastic_hit.phase == phase::COMPRESS)
								{
									elastic_hit.phase = phase::STRETCH;
								}
								else if (elastic_hit.phase == phase::STRETCH)
								{
									elastic_hit.phase = phase::RESUME;
								}

								elastic_hit.phase_elapsed = 0;
							}

							const auto t = elastic_hit.phase_elapsed / elastic_hit.phase_duration;
							switch (elastic_hit.phase)
							{
								case phase::COMPRESS:
								{
									const auto offset = elastic_hit.compress_phase_diff * t;

									// normal_scale = {1, 1}
									// scale.extra = normal_scale + offset;
									scale.extra = sf::Vector2f{1, 1} + offset;
									break;
								}
								case phase::STRETCH:
								{
									const auto offset = elastic_hit.stretch_phase_diff * t;

									scale.extra = elastic_hit.compress_scale + offset;
									break;
								}
								case phase::RESUME:
								{
									const auto offset = elastic_hit.resume_phase_diff * t;

									scale.extra = elastic_hit.stretch_scale + offset;
									break;
								}
								default: // NOLINT(clang-diagnostic-covered-switch-default)
								{
									PROMETHEUS_PLATFORM_UNREACHABLE();
								}
							}
						},
						[&](const entt::entity entity, render_effect::Scale& scale, res::Ripple& ripple) noexcept -> void
						{
							ripple.elapsed += delta;
							if (ripple.elapsed >= ripple.duration)
							{
								// 移除特效
								registry.remove<res::Ripple>(entity);
								// 重置缩放
								scale.extra = sf::Vector2f{0, 0};
								return;
							}

							ripple.radius = ripple.elapsed / ripple.duration;
							// 根据半径插值
							auto t = std::sin(ripple.speed * ripple.duration * ripple.radius * std::numbers::pi_v<float>);
							// 平滑
							t = t * t;
							const auto offset = ripple.scale_diff * t;

							scale.extra = ripple.center_scale + offset;
						},

						// ================================

						[&](this const auto& self, const entt::entity entity, render_effect::Scale& scale) noexcept -> auto
						{
							return [&self, entity, &scale](auto& e) noexcept -> void // NOLINT(clang-diagnostic-padded)
							{
								self(entity, scale, e);
							};
						}
				};

				for (const auto view = registry.view<res::Effect, render_effect::Scale>();
				     const auto [entity, effect, scale]: view.each())
				{
					std::visit(visitor(entity, scale), effect);
				}
			}
		}

		namespace color
		{
			namespace rec = render_effect::color;

			// ReSharper disable once IdentifierTypo
			[[nodiscard]] auto lerp_sd(const sf::Color start, const sf::Color diff, const float t) noexcept -> sf::Color
			{
				PROMETHEUS_PLATFORM_ASSUME(t >= 0 and t <= 1);

				const auto calculate = [&](std::uint8_t sf::Color::* member) noexcept -> std::uint8_t
				{
					const auto s = static_cast<float>(start.*member) / 255.0f;
					const auto d = static_cast<float>(diff.*member) / 255.0f;

					const auto v = s + d * t;
					return static_cast<std::uint8_t>(v * 255.0f);
				};

				const auto red = calculate(&sf::Color::r);
				const auto green = calculate(&sf::Color::g);
				const auto blue = calculate(&sf::Color::b);
				const auto alpha = calculate(&sf::Color::a);

				return {red, green, blue, alpha};
			}

			// // ReSharper disable once IdentifierTypo
			// [[nodiscard]] auto lerp_se(const sf::Color start, const sf::Color end, const float t) noexcept -> sf::Color
			// {
			// 	const auto diff = end - start;
			// 	return lerp_sd(start, diff, t);
			// }

			[[nodiscard]] auto from_hsv(float h, const float s, const float v) noexcept -> sf::Color
			{
				constexpr auto make_color = [](const float r, const float g, const float b) noexcept -> sf::Color
				{
					return
					{
							static_cast<std::uint8_t>(r * 255),
							static_cast<std::uint8_t>(g * 255),
							static_cast<std::uint8_t>(b * 255),
					};
				};

				h = std::fmod(h, 1.0f);
				if (h < 0)
				{
					h += 1;
				}

				const auto i = static_cast<int>(h * 6);
				const auto f = h * 6 - static_cast<float>(i);
				const auto p = v * (1 - s);
				const auto q = v * (1 - f * s);
				const auto t = v * (1 - (1 - f) * s);

				switch (i)
				{
					case 0:
					{
						return make_color(v, t, p);
					}
					case 1:
					{
						return make_color(q, v, p);
					}
					case 2:
					{
						return make_color(p, v, t);
					}
					case 3:
					{
						return make_color(p, q, v);
					}
					case 4:
					{
						return make_color(t, p, v);
					}
					default:
					{
						return make_color(v, p, q);
					}
				}
			}

			auto update(entt::registry& registry, const float delta) noexcept -> void
			{
				const auto visitor = prometheus::functional::overloaded
				{
						[&](const entt::entity entity, render_effect::Color& color, rec::Fade& fade) noexcept -> void
						{
							fade.elapsed += delta;
							if (fade.elapsed >= fade.duration)
							{
								// 移除特效
								registry.remove<rec::Fade>(entity);
								// 重置颜色(需要吗?)
								// color.color = sf::Color::White;
								return;
							}

							const auto t = fade.elapsed / fade.duration;

							color.color = lerp_sd(fade.start, fade.diff, t);
						},
						[&](const entt::entity entity, render_effect::Color& color, rec::AlphaFade& alpha_fade) noexcept -> void
						{
							alpha_fade.elapsed += delta;
							if (alpha_fade.elapsed >= alpha_fade.duration)
							{
								// 移除特效
								registry.remove<rec::AlphaFade>(entity);
								// 重置颜色(需要吗?)
								// color.color = sf::Color::White;
								return;
							}

							const auto t = alpha_fade.elapsed / alpha_fade.duration;
							const auto alpha = alpha_fade.start + alpha_fade.diff * t;

							color.color.a = static_cast<std::uint8_t>(alpha * 255);
						},
						[&](const entt::entity entity, render_effect::Color& color, rec::Flash& flash) noexcept -> void
						{
							flash.elapsed += delta;
							if (flash.elapsed >= flash.duration)
							{
								// 移除特效
								registry.remove<rec::Fade>(entity);
								// 重置颜色
								color.color = sf::Color::White;
								return;
							}

							// 方波闪烁
							const auto t = flash.speed * flash.elapsed;
							if (const auto is_flashing = static_cast<int>(t) % 2 == 0;
								is_flashing)
							{
								auto intensity = 1.0f;
								if (flash.decay > 0)
								{
									intensity = 1.0f - (flash.elapsed / flash.duration) * flash.decay;
								}

								color.color = lerp_sd(flash.base_color, flash.diff, intensity);
							}
							else
							{
								color.color = flash.base_color;
							}
						},
						[&](const entt::entity entity, render_effect::Color& color, rec::Breathing& breathing) noexcept -> void
						{
							breathing.elapsed += delta;
							if (breathing.elapsed >= breathing.duration)
							{
								// 移除特效
								registry.remove<rec::Breathing>(entity);
								// 重置颜色
								color.color = sf::Color::White;
								return;
							}

							// 使用正弦波模拟平滑呼吸
							const auto t = breathing.speed * breathing.elapsed * std::numbers::pi_v<float> * 2;
							const auto normalized = (std::sin(t) + 1.f) / 2.f;

							color.color = lerp_sd(breathing.min, breathing.diff, normalized);
						},
						[&](const entt::entity entity, render_effect::Color& color, rec::Pulse& pulse) noexcept -> void
						{
							pulse.elapsed += delta;
							if (pulse.elapsed >= pulse.duration)
							{
								// 移除特效
								registry.remove<rec::Pulse>(entity);
								// 重置颜色
								color.color = sf::Color::White;
								return;
							}

							// 当前在脉冲周期位置
							const auto pulse_elapsed = std::fmod(pulse.elapsed, pulse.cycle_duration);
							const auto t = [&] noexcept -> float
							{
								if (pulse_elapsed < pulse.raise_duration)
								{
									// 上升阶段
									return pulse_elapsed / pulse.raise_duration;
								}

								if (pulse_elapsed < pulse.raise_duration + pulse.peak_duration)
								{
									// 峰值保持阶段
									return 1;
								}

								// 下降阶段
								return 1 - (pulse_elapsed - pulse.raise_duration - pulse.peak_duration) / pulse.raise_duration;
							}();

							color.color = lerp_sd(pulse.base_color, pulse.diff, t);
						},
						[&](const entt::entity entity, render_effect::Color& color, rec::RainbowCycle& rainbow_cycle) noexcept -> void
						{
							rainbow_cycle.elapsed += delta;
							if (rainbow_cycle.elapsed >= rainbow_cycle.duration)
							{
								// 移除特效
								registry.remove<rec::RainbowCycle>(entity);
								// 重置颜色
								color.color = sf::Color::White;
								return;
							}

							const auto hue = rainbow_cycle.speed * rainbow_cycle.elapsed;

							color.color = from_hsv(hue, rainbow_cycle.saturation, rainbow_cycle.lightness);
						},
						[&](const entt::entity entity, render_effect::Color& color, rec::Oscillator& oscillator) noexcept -> void
						{
							oscillator.elapsed += delta;
							if (oscillator.elapsed >= oscillator.duration)
							{
								// 移除特效
								registry.remove<rec::Oscillator>(entity);
								// 重置颜色
								color.color = sf::Color::White;
								return;
							}

							const auto traveled = oscillator.speed * oscillator.elapsed;
							const auto offset = [&] noexcept -> float
							{
								constexpr auto diff = 1.0f;
								constexpr auto swing = diff * 2;

								const auto mod = std::fmod(traveled, swing);

								if (mod < diff)
								{
									return mod;
								}

								return swing - mod;
							}();

							color.color = lerp_sd(oscillator.color1, oscillator.diff, offset);
						},

						// ================================

						[&](this const auto& self, const entt::entity entity, render_effect::Color& color) noexcept -> auto
						{
							return [&self, entity, &color](auto& e) noexcept -> void // NOLINT(clang-diagnostic-padded)
							{
								self(entity, color, e);
							};
						}
				};

				for (const auto view = registry.view<rec::Effect, render_effect::Color>();
				     const auto [entity, effect, color]: view.each())
				{
					std::visit(visitor(entity, color), effect);
				}
			}
		}
	}

	auto render_effect(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		// FIXME: 渲染特效更新没有考虑动画暂停

		const auto delta_seconds = delta.asSeconds();

		// =========================================
		// POSITION
		// =========================================

		position::update(registry, delta_seconds);

		// =========================================
		// SCALE
		// =========================================

		scale::update(registry, delta_seconds);

		// =========================================
		// COLOR
		// =========================================

		color::update(registry, delta_seconds);
	}
}
