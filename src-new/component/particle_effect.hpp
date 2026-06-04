// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <manager/resource_fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Shader.hpp>

namespace pd::component::particle_effect
{
	// 渲染层配置
	class RenderLayers final
	{
	public:
		// 单层渲染配置
		class Layer final
		{
		public:
			// blueprint::ParticleLayerConfig::sprite

			// 纹理
			manager::texture_handler texture;
			// UV坐标
			sf::Vector2f uv_position;
			// UV尺寸
			sf::Vector2f uv_size;
			// 锚点
			sf::Vector2f pivot;

			// 混合模式
			sf::BlendMode blend_mode;

			// 着色器
			manager::shader_handler shader;

			// 着色器参数
			// 起始颜色
			sf::Color start_color;
			// 结束颜色
			sf::Color end_color;
			// 起始缩放倍率
			float start_scale;
			// 结束缩放倍率
			float end_scale;
		};

		std::vector<Layer> layers;
	};

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

	// 发射器发射粒子生命周期
	class ParticleLifetime final
	{
	public:
		sf::Time lifetime;
	};

	// 发射器发射粒子加速度(像素/秒²)
	class ParticleAcceleration final
	{
	public:
		sf::Vector2f acceleration;
	};

	// 活跃的粒子列表
	class Particles final
	{
	public:
		class Particle final
		{
		public:
			// 世界位置(像素)
			sf::Vector2f position;
			// 速度(像素/秒)
			sf::Vector2f velocity;
			// 已历时
			sf::Time elapsed;
		};

		std::vector<Particle> particles;
	};
}
