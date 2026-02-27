// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

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
		// 为一个实体附加动画组件
		// 如果目标实体已经存在动画组件则会进行覆盖
		static auto attach(entt::registry& registry, entt::entity entity, const config::Animation& animation) noexcept -> void;

		// 为一个实体移除动画组件
		static auto deattach(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 暂停指定实体的动画
		static auto pause(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 恢复指定实体的动画
		static auto unpause(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 使指定实体的动画循环
		static auto looping(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;

		// 使指定实体的动画不再循环(依然会播放到最后一帧,然后不再更新)
		static auto unlooping(entt::registry& registry, entt::entity entity_with_animation) noexcept -> void;
	};
}
