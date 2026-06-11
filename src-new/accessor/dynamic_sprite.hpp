// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/def.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::accessor
{
	class DynamicSprite final
	{
	public:
		using index_type = std::size_t;

		// 如果动画不循环且播放到最后一帧,则next_frame返回此值
		constexpr static index_type animation_ended = 0xffffffff;

		// ===========================================================================
		// FRAME
		// ===========================================================================

		// 获取实体的动画的总帧数
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto get_frame_count(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> index_type;

		// 获取实体的动画的下一帧动画索引
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto get_next_frame_index(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// 假定实体存在动画组件
		// 会更新Timer组件,即重置当前帧的计时(如果当前帧快结束时跳转到下一帧,则至少会保证在新帧停留duration_ms的时间)
		static auto set_next_frame(entt::registry& registry, entt::entity entity_with_animation) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// 假定实体存在动画组件
		// 不会更新Timer组件,即不会重置当前帧的计时(如果当前帧快结束时跳转到下一帧,则可能会很快就进入下一帧)
		static auto jump_to_next_frame(entt::registry& registry, entt::entity entity_with_animation) noexcept -> index_type;

		// 使实体的动画播放到第N帧
		//
		// 假定实体存在动画组件
		// 会更新Timer组件,即重置当前帧的计时(如果当前帧快结束时跳转到其他帧,则至少会保证在新帧停留duration_ms的时间)
		// 如果frame_index小于动画总帧数则返回frame_index,否则返回frame_index%frames_count
		static auto set_frame(entt::registry& registry, entt::entity entity_with_animation, index_type frame_index) noexcept -> index_type;

		// 使实体的动画播放到第N帧
		//
		// 假定实体存在动画组件
		// 不会更新Timer组件,即不会重置当前帧的计时(如果当前帧快结束时跳转到其他帧,则可能会很快就进入下一帧)
		// 如果frame_index小于动画总帧数则返回frame_index,否则返回frame_index%frames_count
		static auto jump_to_frame(entt::registry& registry, entt::entity entity_with_animation, index_type frame_index) noexcept -> index_type;

		// ===========================================================================
		// MODE
		// ===========================================================================

		// 设置实体的动画的播放模式
		//
		// 假定实体存在动画组件
		static auto set_mode(
			entt::registry& registry,
			entt::entity entity_with_animation,
			blueprint::AnimationMode mode
		) noexcept -> void;

		// 获取实体的动画的播放模式
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto get_mode(
			const entt::registry& registry,
			entt::entity entity_with_animation
		) noexcept -> blueprint::AnimationMode;

		// 设置实体的动画的播放模式为循环
		// set_mode(LOOP)
		//
		// 假定实体存在动画组件
		static auto set_mode_loop(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 设置实体的动画的播放模式为单次
		// set_mode(ONE_SHOT)
		//
		// 假定实体存在动画组件
		static auto set_mode_one_shot(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查实体的动画的播放模式是否为循环
		// get_mode() == LOOP
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_loop_mode(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 检查实体的动画的播放模式是否为单次
		// get_mode() == ONE_SHOT
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_one_shot_mode(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// ===========================================================================
		// DIRECTION
		// ===========================================================================

		// 设置实体的动画的播放方向
		static auto set_direction(
			entt::registry& registry,
			entt::entity entity_with_animation,
			blueprint::AnimationDirection direction
		) noexcept -> void;

		// 获取实体的动画的播放方向
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto get_direction(
			const entt::registry& registry,
			entt::entity entity_with_animation
		) noexcept -> blueprint::AnimationDirection;

		// 设置实体的动画的播放方向为正向
		// set_direction(FORWARD)
		//
		// 假定实体存在动画组件
		static auto set_direction_forward(
			entt::registry& registry,
			entt::entity entity_with_animation
		) noexcept -> void;

		// 设置实体的动画的播放方向为反向
		// set_direction(BACKWARD)
		//
		// 假定实体存在动画组件
		static auto set_direction_backward(
			entt::registry& registry,
			entt::entity entity_with_animation
		) noexcept -> void;

		// 检查实体的动画的播放方向是否为正向
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_forward_direction(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 检查实体的动画的播放方向是否为反向
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_backward_direction(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// ===========================================================================
		// STATE
		// ===========================================================================

		// ===========
		// PLAYING
		// ===========

		// 检查实体的动画是否处于播放状态
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_playing(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// ===========
		// PAUSED
		// ===========

		// 检查实体的动画是否处于暂停状态
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_paused(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 使实体的动画暂停
		//
		// 假定实体存在动画组件
		static auto pause(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 使实体的动画恢复
		//
		// 假定实体存在动画组件
		static auto resume(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// ===========
		// ENDED
		// ===========

		// 检查实体的动画是否处于结束状态
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_ended(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 使实体的动画结束
		// 仅添加结束标记,并不实际结束动画
		//
		// 假定实体存在动画组件
		static auto end(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 使实体的动画重新播放
		// 基于方向:
		//  正向: 从第一帧开始播放
		//  反向: 从最后一帧开始播放
		//
		// 假定实体存在动画组件
		static auto replay(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;
	};
}
