// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <scene/scene.hpp>

#include <components/game.hpp>

namespace pd::scene
{
	Scene::Scene(Game& game) noexcept
	{
		// 保存游戏实例引用
		// 如果派生类会用到游戏实例,其必须保证调用点位于基类构造函数之后
		registry_.ctx().emplace<components::Game>(std::ref(game));
	}

	Scene::~Scene() noexcept = default;
}
