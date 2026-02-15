// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <ctx/animation.hpp>

#include <components/animation.hpp>

#include <entt/entt.hpp>

namespace pd::ctx
{
	auto Animation::add_set(entt::registry& registry, blueprint::AnimationSet&& sub_set) noexcept -> void
	{
		auto& set = registry.ctx().get<blueprint::AnimationSet>();

		set.merge(std::move(sub_set));
	}

	auto Animation::get_set(entt::registry& registry) noexcept -> const blueprint::AnimationSet&
	{
		return registry.ctx().get<const blueprint::AnimationSet>();
	}

	auto Animation::get(entt::registry& registry, const std::string_view name) noexcept -> blueprint::AnimationView
	{
		const auto& set = registry.ctx().get<const blueprint::AnimationSet>();

		if (const auto it = set.find(name);
			it != set.end())
		{
			return it->second;
		}

		// todo: 备用动画?
		return set.begin()->second;
	}

	auto Animation::pause(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components::animation;

		registry.emplace_or_replace<Paused>(entity);
	}

	auto Animation::unpause(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components::animation;

		registry.remove<Paused>(entity);
	}

	auto Animation::looping(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components::animation;

		registry.emplace_or_replace<Looping>(entity);

		// 如果非循环动画之前已经播放完毕,则需要移除已结束的标记
		registry.remove<Ended>(entity);
	}

	auto Animation::unlooping(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components::animation;

		registry.remove<Looping>(entity);
	}
}
