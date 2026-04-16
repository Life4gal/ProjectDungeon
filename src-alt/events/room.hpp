// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>
#include <functional>

#include <entt/entity/fwd.hpp>

namespace pd::events::room
{
	namespace door
	{
		// 禁用指定门实体
		class Disable final
		{
		public:
			entt::entity door;
		};

		// 启用指定门实体
		class Enable final
		{
		public:
			entt::entity door;
		};

		// 禁用所有指定门实体
		class DisableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};

		// 启用所有指定门实体
		class EnableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};

		// 请求开启所有门(如果可以开启)
		class OpenRequest final {};

		// 请求关闭所有门(如果可以关闭)
		class CloseRequest final {};

		// 任意实体与门实体接触
		class Contacted final
		{
		public:
			entt::entity door;
			entt::entity entity;
		};

		// 任意实体与门传感器接触
		class SensorContacted final
		{
		public:
			entt::entity door;
			entt::entity entity;
		};
	}

	namespace item
	{
		// 禁用指定物品实体
		class Disable final
		{
		public:
			entt::entity door;
		};

		// 启用指定物品实体
		class Enable final
		{
		public:
			entt::entity door;
		};

		// 禁用所有指定物品实体
		class DisableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};

		// 启用所有指定物品实体
		class EnableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};
	}

	namespace chest
	{
		// 禁用指定宝箱实体
		class Disable final
		{
		public:
			entt::entity chest;
		};

		// 启用指定宝箱实体
		class Enable final
		{
		public:
			entt::entity chest;
		};

		// 禁用所有指定宝箱实体
		class DisableAll final
		{
		public:
			std::span<const entt::entity> chests;
		};

		// 启用所有指定宝箱实体
		class EnableAll final
		{
		public:
			std::span<const entt::entity> chests;
		};

		// 任意实体与宝箱实体接触
		class Contacted final
		{
		public:
			entt::entity chest;
			entt::entity entity;
		};
	}

	namespace blood_stain
	{
		// 禁用指定血迹实体
		class Disable final
		{
		public:
			entt::entity door;
		};

		// 启用指定血迹实体
		class Enable final
		{
		public:
			entt::entity door;
		};

		// 禁用所有指定血迹实体
		class DisableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};

		// 启用所有指定血迹实体
		class EnableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};
	}

	namespace corpse
	{
		// 禁用指定尸体实体
		class Disable final
		{
		public:
			entt::entity door;
		};

		// 启用指定尸体实体
		class Enable final
		{
		public:
			entt::entity door;
		};

		// 禁用所有指定尸体实体
		class DisableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};

		// 启用所有指定尸体实体
		class EnableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};
	}

	namespace destroyable
	{
		// 禁用指定可破坏物实体
		class Disable final
		{
		public:
			entt::entity door;
		};

		// 启用指定可破坏物实体
		class Enable final
		{
		public:
			entt::entity door;
		};

		// 禁用所有指定可破坏物实体
		class DisableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};

		// 启用所有指定可破坏物实体
		class EnableAll final
		{
		public:
			std::span<const entt::entity> doors;
		};
	}

	// 进入指定房间
	class Entered final
	{
	public:
		entt::entity room;
	};

	// 指定房间已清理
	class Cleared final
	{
	public:
		entt::entity room;
	};

	// 离开指定房间
	class Left final
	{
	public:
		entt::entity room;
	};
}
