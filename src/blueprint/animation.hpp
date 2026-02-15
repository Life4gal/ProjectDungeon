// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/animation.hpp>

namespace pd::blueprint
{
	// TODO: 
	//  1.AnimationFrame使用纹理路径,这里如果转换为纹理ID则不需要每次获取纹理都要计算一次ID
	//  2.AnimationSet使用字符串动画名,这里如果转换为动画ID则不需要每次获取动画都要计算一次ID
	//  3.load_animation当前主需要传入config::AnimationSet,因为AnimationFrame会保存纹理路径,
	//  如果转换为纹理ID则需要再传入一个映射表,接收纹理路径,返回纹理ID

	// 一帧动画所包含的数据(对应config::AnimationFrame)
	class AnimationFrame final
	{
	public:
		// 该帧使用纹理路径(通常来说会使用同一张纹理)
		std::filesystem::path texture_path;
		// 该帧持续时间
		std::chrono::microseconds duration;
		// 该帧纹理位置
		int frame_x;
		int frame_y;
		// 该帧纹理大小
		int frame_width;
		int frame_height;
		// 总是假定原点为正中心(width/2, height/2)
	};

	// 完整动画所包含的数据(对应config::Animation)
	class Animation final
	{
	public:
		using frames_type = std::vector<AnimationFrame>;

		// 所有帧
		frames_type frames;
		// 是否循环播放
		bool looping;
	};

	// Animation只读视图
	class AnimationView final : public std::reference_wrapper<const Animation>
	{
	public:
		using reference_wrapper::reference_wrapper;

		[[nodiscard]] constexpr auto operator*() const noexcept -> const Animation&
		{
			return get();
		}

		[[nodiscard]] constexpr auto operator->() const noexcept -> const Animation*
		{
			return std::addressof(get());
		}
	};

	// 动画集(对应config::AnimationSet)
	// 动画名称<->动画数据
	class AnimationSet final : public std::unordered_map<
				std::string,
				Animation,
				utility::StringHash,
				std::ranges::equal_to
			>
	{
	public:
		using unordered_map::unordered_map;
	};

	// 将config数据转换为蓝图数据
	[[nodiscard]] auto load_animation(const config::AnimationSet& data) noexcept -> AnimationSet;
}
