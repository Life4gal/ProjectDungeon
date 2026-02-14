// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/blueprint.hpp>

#include <algorithm>
#include <ranges>

#include <game/names.hpp>

// =====================
// CONFIG
// =====================

#include <config/flip_animation.hpp>

// =====================
// BLUEPRINT
// =====================

#include <blueprint/flip_animation.hpp>

// =====================
// ASSET
// =====================

#include <asset/font_loader.hpp>
#include <asset/texture_loader.hpp>
#include <asset/sound_loader.hpp>
#include <asset/music_loader.hpp>

// =====================
// DEPENDENCIES
// =====================

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::systems
{
	namespace
	{
		// 从文件中读取动画数据
		[[nodiscard]] auto load_flip_animations_data() noexcept -> config::flip_animations_type
		{
			using namespace config;
			using namespace std::chrono_literals;

			flip_animations_type animations{};

			// AntleredRascal
			{
				auto& frames = animations["AntleredRascal"];

				// 第1帧
				frames.emplace_back(
					FlipAnimationFrame
					{
							.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
							.duration = 250ms,
							.frame_x = 0,
							.frame_y = 0,
							.frame_width = 16,
							.frame_height = 16,
					}
				);
				// 第2帧
				frames.emplace_back(
					FlipAnimationFrame
					{
							.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
							.duration = 250ms,
							.frame_x = 16,
							.frame_y = 0,
							.frame_width = 16,
							.frame_height = 16,
					}
				);
				// 第3帧
				frames.emplace_back(
					FlipAnimationFrame
					{
							.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
							.duration = 250ms,
							.frame_x = 32,
							.frame_y = 0,
							.frame_width = 16,
							.frame_height = 16,
					}
				);
				// 第4帧
				frames.emplace_back(
					FlipAnimationFrame
					{
							.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
							.duration = 250ms,
							.frame_x = 48,
							.frame_y = 0,
							.frame_width = 16,
							.frame_height = 16,
					}
				);
			}

			return animations;
		}

		// 将config数据转换为蓝图数据
		auto load_flip_animation_blueprint(blueprint::flip_animations_type& out_animations, asset::map_type& out_texture_map) noexcept -> void
		{
			auto animations_data = load_flip_animations_data();

			out_animations.reserve(animations_data.size());
			// 具体纹理数量未知,不过map不reserve问题也不大

			for (auto&& [animation_name, animation_frames]: animations_data/* | std::views::as_rvalue*/)
			{
				blueprint::flip_animation_frames_type frames{};
				frames.reserve(animation_frames.size());

				for (auto&& frame: animation_frames /* | std::views::as_rvalue*/)
				{
					// 注册纹理
					const auto texture_id = out_texture_map.id_of(frame.texture_path);

					frames.emplace_back(
						texture_id,
						frame.duration,
						frame.frame_x,
						frame.frame_y,
						frame.frame_width,
						frame.frame_height
					);
				}

				out_animations.emplace(animation_name, std::move(frames));
			}
		}
	}

	auto Blueprint::do_loaded(entt::registry& registry) noexcept -> void
	{
		// 创建map
		// 这些实体由systems::Asset使用
		auto& font_map = registry.ctx().emplace_as<asset::map_type>(Names::asset_font);
		auto& texture_map = registry.ctx().emplace_as<asset::map_type>(Names::asset_texture);
		auto& sound_map = registry.ctx().emplace_as<asset::map_type>(Names::asset_sound);
		auto& music_map = registry.ctx().emplace_as<asset::map_type>(Names::asset_music);

		// 创建animations
		// 这些实体由systems::FlipAnimation使用
		auto& animations = registry.ctx().emplace_as<blueprint::flip_animations_type>(Names::blueprint_flip_animation);

		// 载入翻页动画资源
		load_flip_animation_blueprint(animations, texture_map);
	}

	auto Blueprint::do_initialized([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Blueprint::do_unloaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Blueprint::do_update([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		// 什么也不做
	}

	auto Blueprint::do_render([[maybe_unused]] entt::registry& registry, [[maybe_unused]] sf::RenderWindow& window) noexcept -> void
	{
		// 什么也不做
	}
}
