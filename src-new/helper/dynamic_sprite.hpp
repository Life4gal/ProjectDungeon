// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/render.hpp>

namespace pd::helper
{
	class DynamicSprite final
	{
	public:
		using index_type = std::size_t;

		// 如果动画不循环且播放到最后一帧,则next_frame返回此值
		constexpr static index_type animation_ended = 0xffffffff;

		// ===========================================================================
		// INLINE OPERATION
		// ===========================================================================

		// 获取实体的动画的下一帧动画索引
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		[[nodiscard]] static auto get_next_frame_index(
			const component::render::dynamic_sprite::BeginFrameIndex& begin_frame,
			const component::render::dynamic_sprite::EndFrameIndex& end_frame,
			const component::render::dynamic_sprite::CurrentFrameIndex& index,
			const component::render::dynamic_sprite::AnimationMode& mode,
			const component::render::dynamic_sprite::AnimationDirection& direction
		) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		// *不会*标记动画结束(显然如此,因为未传入registry&entity)
		static auto set_next_frame(
			const component::render::dynamic_sprite::BeginFrameIndex& begin_frame,
			const component::render::dynamic_sprite::EndFrameIndex& end_frame,
			component::render::dynamic_sprite::CurrentFrameIndex& index,
			component::render::dynamic_sprite::CurrentFrameElapsed& elapsed,
			const component::render::dynamic_sprite::AnimationMode& mode,
			const component::render::dynamic_sprite::AnimationDirection& direction
		) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		// *不会*标记动画结束(显然如此,因为未传入registry&entity)
		static auto jump_to_next_frame(
			const component::render::dynamic_sprite::BeginFrameIndex& begin_frame,
			const component::render::dynamic_sprite::EndFrameIndex& end_frame,
			component::render::dynamic_sprite::CurrentFrameIndex& index,
			const component::render::dynamic_sprite::AnimationMode& mode,
			const component::render::dynamic_sprite::AnimationDirection& direction
		) noexcept -> index_type;

		// 使实体的动画播放到第N帧
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		static auto set_frame(
			index_type frame_index,
			const component::render::dynamic_sprite::BeginFrameIndex& begin_frame,
			const component::render::dynamic_sprite::EndFrameIndex& end_frame,
			component::render::dynamic_sprite::CurrentFrameIndex& index,
			component::render::dynamic_sprite::CurrentFrameElapsed& elapsed
		) noexcept -> index_type;

		// 使实体的动画播放到第N帧
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		static auto jump_to_frame(
			index_type frame_index,
			const component::render::dynamic_sprite::BeginFrameIndex& begin_frame,
			const component::render::dynamic_sprite::EndFrameIndex& end_frame,
			component::render::dynamic_sprite::CurrentFrameIndex& index
		) noexcept -> index_type;
	};
}
