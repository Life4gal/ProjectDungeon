// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>
#include <vector>

#include <blueprint/detail/transform.hpp>

namespace pd::blueprint
{
	namespace ai_detail
	{
		// 移动行为
		class MoveBehavior final
		{
		public:
			// 不移动
			class Stationary final {};

			// 漫游(随机移动)
			class Wander final
			{
			public:
				// 移动速度
				float speed;
				// 考虑下一次转向的最小时间
				float next_turn_min_time;
				// 考虑下一次转向的最大时间
				float next_turn_max_timer;
				// 
			};

			// 巡逻
			class Patrol final
			{
			public:
				// waypoints[0] -> waypoints[1] -> waypoints[2] -> ... -> waypoints[n-2] -> waypoints[n-1] -> waypoints[0]
				std::vector<Position> waypoints;
			};

			// 追逐玩家
			class Chase final
			{
			public:
				// 移动速度
				float speed;
			};

			// 跳跃(随机移动)
			class Jump final
			{
			public:
				// TODO
			};

			// 跳跃(追逐玩家)
			class ChaseJump final
			{
			public:
				// 跳跃速度
				float speed;
				// 跳跃持续时间
				float duration;
				// 考虑下一次跳跃的最小时间
				float next_jump_min_time;
				// 考虑下一次跳跃的最大时间
				float next_jump_max_time;
			};

			// 传送
			class Teleport final
			{
			public:
				// TODO
			};
		};
	}

	// 移动行为
	class MoveBehavior final : public std::variant<
				ai_detail::MoveBehavior::Stationary,
				ai_detail::MoveBehavior::Wander,
				ai_detail::MoveBehavior::Patrol,
				ai_detail::MoveBehavior::Chase,
				ai_detail::MoveBehavior::Jump,
				ai_detail::MoveBehavior::ChaseJump,
				ai_detail::MoveBehavior::Teleport
			>
	{
	public:
		using stationary = ai_detail::MoveBehavior::Stationary;
		using wander = ai_detail::MoveBehavior::Wander;
		using patrol = ai_detail::MoveBehavior::Patrol;
		using chase = ai_detail::MoveBehavior::Chase;
		using jump = ai_detail::MoveBehavior::Jump;
		using chase_jump = ai_detail::MoveBehavior::ChaseJump;
		using teleport = ai_detail::MoveBehavior::Teleport;

		using variant::variant;
	};

	class Ai final
	{
	public:
		// 移动行为
		MoveBehavior move_behavior;

		//
	};
}
