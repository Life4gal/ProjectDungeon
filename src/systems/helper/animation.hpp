// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <limits>

#include <entt/fwd.hpp>

namespace pd::config
{
	class Animation;
}

namespace pd::systems::helper
{
	class Animation final
	{
	public:
		// 如果动画不循环且播放到最后一帧,则next_frame返回此值
		constexpr static auto animation_ended = std::numeric_limits<std::size_t>::max();

		// 为一个实体附加动画组件
		// 如果目标实体已经存在动画组件则会进行覆盖
		static auto attach(entt::registry& registry, entt::entity entity, const config::Animation& animation) noexcept -> void;

		// 为一个实体移除动画组件
		static auto deattach(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 获取指定实体的动画纹理宽度
		[[nodiscard]] static auto get_texture_width(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> int;

		// 获取指定实体的动画纹理高度
		[[nodiscard]] static auto get_texture_height(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> int;

		// 获取指定实体的动画原点X坐标
		[[nodiscard]] static auto get_origin_x(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> int;

		// 获取指定实体的动画原点Y坐标
		[[nodiscard]] static auto get_origin_y(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> int;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		// 这只会更新Animation组件,即动画的帧索引,不会更新Render组件(即其引用的动画帧)
		// 会更新Timer组件,即重置当前帧的计时(如果当前帧快结束时跳转到下一帧,则至少会保证在新帧停留duration_ms的时间)
		[[nodiscard]] static auto set_next_frame(entt::registry& registry, entt::entity entity_with_animation) noexcept -> std::size_t;

		// 使实体的动画播放到下一帧(返回下一帧动画的索引)
		// 这只会更新Animation组件,即动画的帧索引,不会更新Render组件(即其引用的动画帧)
		// 不会更新Timer组件,即不会重置当前帧的计时(如果当前帧快结束时跳转到下一帧,则可能会很快就进入下一帧)
		[[nodiscard]] static auto jump_to_next_frame(entt::registry& registry, entt::entity entity_with_animation) noexcept -> std::size_t;

		// 使实体的动画播放到第N帧
		// 这只会更新Animation组件,即动画的帧索引,不会更新Render组件(即其引用的动画帧)
		// 会更新Timer组件,即重置当前帧的计时(如果当前帧快结束时跳转到其他帧,则至少会保证在新帧停留duration_ms的时间)
		static auto set_frame(entt::registry& registry, entt::entity entity_with_animation, std::size_t frame_index) noexcept -> void;

		// 使实体的动画播放到第N帧
		// 这只会更新Animation组件,即动画的帧索引,不会更新Render组件(即其引用的动画帧)
		// 不会更新Timer组件,即不会重置当前帧的计时(如果当前帧快结束时跳转到其他帧,则可能会很快就进入下一帧)
		static auto jump_to_frame(entt::registry& registry, entt::entity entity_with_animation, std::size_t frame_index) noexcept -> void;

		// 使指定实体的动画循环
		static auto looping(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 使指定实体的动画不再循环(依然会播放到最后一帧,然后不再更新)
		static auto unlooping(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查指定实体的动画是否循环
		[[nodiscard]] static auto is_looping(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 使指定实体的动画反向播放
		static auto reverse(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查指定实体的动画是否反向播放
		[[nodiscard]] static auto is_reversed(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 暂停指定实体的动画
		static auto pause(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 恢复指定实体的动画
		static auto unpause(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 检查指定实体的动画是否暂停
		[[nodiscard]] static auto is_paused(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;

		// 检查指定的实体的动画是否结束
		[[nodiscard]] static auto is_ended(const entt::registry& registry, entt::entity entity_with_animation) noexcept -> bool;
	};
}
