// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/particle.hpp>

#include <SFML/System/Time.hpp>

namespace pd::component::particle_emitter
{
	// 发射器工作时间
	class TotalWorkingTime final
	{
	public:
		sf::Time time;
	};

	// 发射器已工作时间
	class WorkingTime final
	{
	public:
		sf::Time time;
	};

	// 发射器发射粒子间隔
	class EmissionInterval final
	{
	public:
		sf::Time interval;
	};

	// 发射器发射粒子已经历间隔
	class EmissionCooldown final
	{
	public:
		sf::Time cooldown;
	};

	// 粒子蓝图
	class Particle final
	{
	public:
		blueprint::Particle particle;
	};
}
