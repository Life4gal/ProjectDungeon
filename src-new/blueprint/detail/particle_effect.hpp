// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <vector>

#include <blueprint/detail/render.hpp>

namespace pd::blueprint
{
	// 粒子混合模式
	enum class ParticleBlendMode : std::uint8_t
	{
		// sf::BlendAlpha
		ALPHA,
		// sf::BlendAdd
		ADD,
		// sf::BlendMultiply
		MULTIPLY,
	};

	// 单层粒子配置
	class ParticleLayerConfig final
	{
	public:
		// 纹理
		StaticSprite sprite;
		// 混合模式
		ParticleBlendMode blend_mode;

		// 着色器路径
		std::string shader_path;

		// 起始颜色(uniform: u_start_color)
		std::uint32_t start_color;
		// 结束颜色(uniform: u_end_color)
		std::uint32_t end_color;
		// 起始缩放倍率
		float start_scale;
		// 结束缩放倍率
		float end_scale;
	};

	// 粒子发射器配置
	class ParticleEmitterConfig final
	{
	public:
		// 渲染层列表
		std::vector<ParticleLayerConfig> layers;

		// 发射器工作时间(毫秒)
		// 发射器工作时间结束后将不再发射粒子
		// 粒子依然会继续更新
		int working_time_ms;
		// 发射器发射粒子间隔(毫秒)
		int emission_interval_ms;
		// 发射器发射粒子生命周期(毫秒)
		int particle_lifetime_ms;
		// 发射器发射粒子加速度(像素/秒²)
		Position particle_acceleration;
	};
}
