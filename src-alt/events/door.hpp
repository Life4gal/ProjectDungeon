// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>

#include <entt/entity/fwd.hpp>

namespace pd::events::door
{
	// 禁用目标门实体
	class Disable final
	{
	public:
		entt::entity door;
	};

	// 启用目标门实体
	class Enable final
	{
	public:
		entt::entity door;
	};

	// 禁用所有目标门实体
	class DisableAll final
	{
	public:
		std::span<const entt::entity> doors;
	};

	// 启用所有目标门实体
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
