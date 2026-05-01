// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/sprite_animation.hpp>

#include <entt/fwd.hpp>

namespace pd::helper
{
	class SpriteAnimation final
	{
	public:
		using index_type = std::size_t;

		// 如果动画不循环且播放到最后一帧,则next_frame返回此值
		constexpr static index_type animation_ended = 0xffffffff;

		// 获取实体的下一帧动画索引
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		[[nodiscard]] static auto get_next_frame_index(
			const component::sprite_animation::Total& total,
			const component::sprite_animation::Index& index,
			const component::sprite_animation::Mode& mode,
			const component::sprite_animation::Direction& direction
		) noexcept -> index_type;

		// 获取实体的下一帧动画索引
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto get_next_frame_index(entt::registry& registry, entt::entity entity_with_animation) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		// *不会*标记动画结束(显然如此,因为未传入registry&entity)
		static auto set_next_frame(
			const component::sprite_animation::Sprites& sprites,
			const component::sprite_animation::Total& total,
			component::sprite_animation::Timer& timer,
			component::sprite_animation::Index& index,
			const component::sprite_animation::Mode& mode,
			const component::sprite_animation::Direction& direction
		) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// 假定实体存在动画组件
		// 会更新Timer组件,即重置当前帧的计时(如果当前帧快结束时跳转到下一帧,则至少会保证在新帧停留duration_ms的时间)
		static auto set_next_frame(entt::registry& registry, entt::entity entity_with_animation) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		// *不会*标记动画结束(显然如此,因为未传入registry&entity)
		static auto jump_to_next_frame(
			const component::sprite_animation::Total& total,
			component::sprite_animation::Index& index,
			const component::sprite_animation::Mode& mode,
			const component::sprite_animation::Direction& direction
		) noexcept -> index_type;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		//
		// 假定实体存在动画组件
		// 不会更新Timer组件,即不会重置当前帧的计时(如果当前帧快结束时跳转到下一帧,则可能会很快就进入下一帧)
		static auto jump_to_next_frame(entt::registry& registry, entt::entity entity_with_animation) noexcept -> index_type;

		// 使实体的动画播放到第N帧
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		static auto set_frame(
			index_type frame_index,
			const component::sprite_animation::Sprites& sprites,
			const component::sprite_animation::Total& total,
			component::sprite_animation::Timer& timer,
			component::sprite_animation::Index& index
		) noexcept -> void;

		// 使实体的动画播放到第N帧
		//
		// 假定实体存在动画组件
		// 会更新Timer组件,即重置当前帧的计时(如果当前帧快结束时跳转到其他帧,则至少会保证在新帧停留duration_ms的时间)
		static auto set_frame(entt::registry& registry, entt::entity entity_with_animation, index_type frame_index) noexcept -> void;

		// 使实体的动画播放到第N帧
		//
		// *inline*版本,配合registry.view<xxx>效率更高(不需要多次查找)
		static auto jump_to_frame(
			index_type frame_index,
			const component::sprite_animation::Total& total,
			component::sprite_animation::Index& index
		) noexcept -> index_type;

		// 使实体的动画播放到第N帧
		//
		// 假定实体存在动画组件
		// 不会更新Timer组件,即不会重置当前帧的计时(如果当前帧快结束时跳转到其他帧,则可能会很快就进入下一帧)
		// 如果frame_index小于动画总帧数则返回frame_index,否则返回frame_index%total
		static auto jump_to_frame(entt::registry& registry, entt::entity entity_with_animation, index_type frame_index) noexcept -> index_type;

		// 使指定实体的动画循环
		//
		// 假定实体存在动画组件
		static auto looping(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 使指定实体的动画不再循环(依然会播放到最后一帧,然后不再更新)
		//
		// 假定实体存在动画组件
		static auto unlooping(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查指定实体的动画是否循环
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_looping(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 使指定实体的动画反向播放
		//
		// 假定实体存在动画组件
		static auto reverse(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查指定实体的动画是否反向播放
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_reversed(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 暂停指定实体的动画
		//
		// 假定实体存在动画组件
		static auto pause(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 恢复指定实体的动画
		//
		// 假定实体存在动画组件
		static auto unpause(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查指定实体的动画是否暂停
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_paused(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 停止指定实体的动画(停止在当前帧)
		//
		// 假定实体存在动画组件
		static auto end(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查指定的实体的动画是否结束
		//
		// 假定实体存在动画组件
		[[nodiscard]] static auto is_ended(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;
	};
}
