// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/blueprint.hpp>

#include <algorithm>
#include <ranges>

// =====================
// CONFIG
// =====================

#include <config/animation.hpp>

// =====================
// CTX
// =====================

#include <ctx/asset_loader.hpp>
#include <ctx/animation.hpp>

// =====================
// BLUEPRINT
// =====================

#include <blueprint/animation.hpp>

// =====================
// DEPENDENCIES
// =====================

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::systems::initialize
{
	namespace
	{
		[[nodiscard]] auto load_animation_data() noexcept -> config::animations_type;

		auto load_animation_blueprint(entt::registry& registry) noexcept -> void
		{
			// 将动画用到的纹理资源添加到Map中(随后由TextureLoader加载)
			auto& texture_path_map = ctx::Map<asset::TextureLoader>::get(registry);
			// 动画蓝图资源
			auto& animations_map = ctx::Animations::emplace(registry);

			// 从文件载入动画数据
			auto data = load_animation_data();

			// 转换为蓝图
			animations_map.reserve(data.size());
			std::ranges::for_each(
				data | std::views::as_rvalue,
				[&](auto&& animation) noexcept -> void
				{
					blueprint::animation_frames_type frames{};
					frames.reserve(animation.second.size());

					std::ranges::for_each(
						animation.second | std::views::as_rvalue,
						[&](auto&& frame) noexcept -> void
						{
							// 注册用到的纹理
							const auto texture_id = texture_path_map.id_of(frame.texture_path);

							frames.emplace_back(
								texture_id,
								frame.duration,
								frame.frame_x,
								frame.frame_y,
								frame.frame_width,
								frame.frame_height
							);
						}
					);

					// 保存该动画
					animations_map.emplace(std::move(animation.first), std::move(frames));
				}
			);
		}
	}

	auto blueprint(entt::registry& registry) noexcept -> void
	{
		ctx::Map<asset::FontLoader>::emplace(registry);
		ctx::Map<asset::TextureLoader>::emplace(registry);
		ctx::Map<asset::SoundLoader>::emplace(registry);
		ctx::Map<asset::MusicLoader>::emplace(registry);

		// 目前仅有动画资源
		load_animation_blueprint(registry);
	}

	namespace
	{
		auto load_animation_data() noexcept -> config::animations_type
		{
			using namespace config;
			using namespace std::chrono_literals;

			animations_type animations{};

			// AntleredRascal
			{
				auto& frames = animations["AntleredRascal"];

				// 第1帧
				frames.emplace_back(
					AnimationFrame
					{
							.texture_path = "./media/enemy/deep-dive-AntleredRascal.png",
							.duration = 250ms,
							.frame_x = 0,
							.frame_y = 0,
							.frame_width = 16,
							.frame_height = 16,
					}
				);
				// 第2帧
				frames.emplace_back(
					AnimationFrame
					{
							.texture_path = "./media/enemy/deep-dive-AntleredRascal.png",
							.duration = 250ms,
							.frame_x = 16,
							.frame_y = 0,
							.frame_width = 16,
							.frame_height = 16,
					}
				);
				// 第3帧
				frames.emplace_back(
					AnimationFrame
					{
							.texture_path = "./media/enemy/deep-dive-AntleredRascal.png",
							.duration = 250ms,
							.frame_x = 32,
							.frame_y = 0,
							.frame_width = 16,
							.frame_height = 16,
					}
				);
				// 第4帧
				frames.emplace_back(
					AnimationFrame
					{
							.texture_path = "./media/enemy/deep-dive-AntleredRascal.png",
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
	}
}
