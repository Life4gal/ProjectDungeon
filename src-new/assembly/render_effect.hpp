// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace pd::assembly
{
	class RenderEffect final
	{
	public:
		static auto make(entt::registry& registry, entt::entity entity) noexcept -> void;

		class Position final
		{
		public:
			// 线性
			static auto linear(
				entt::registry& registry,
				entt::entity entity,
				// 起始位置
				sf::Vector2f start,
				// 终点位置
				sf::Vector2f end,
				// 速度(像素/秒)
				float speed
			) noexcept -> void;

			// 震荡
			static auto oscillator(
				entt::registry& registry,
				entt::entity entity,
				// 震荡点1
				sf::Vector2f p1,
				// 震荡点2
				sf::Vector2f p2,
				// 速度(像素/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 弹簧
			static auto spring(
				entt::registry& registry,
				entt::entity entity,
				// 起始位置
				sf::Vector2f start,
				// 目标位置
				sf::Vector2f target,
				// 弹簧强度(像素/秒²)
				float strength,
				// 阻尼系数
				float damping,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 路径
			static auto path(
				entt::registry& registry,
				entt::entity entity,
				// 所有路径点
				std::vector<sf::Vector2f> waypoints,
				// 速度(像素/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 轨道环绕
			static auto orbit(
				entt::registry& registry,
				entt::entity entity,
				// 环绕中心
				sf::Vector2f center,
				// 半径
				float radius,
				// 角速度(弧度/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 抖动/摇晃
			static auto shake(
				entt::registry& registry,
				entt::entity entity,
				// 中心点
				sf::Vector2f center,
				// 抖动强度(像素)
				float intensity,
				// 抖动频率(次/秒)
				float frequency,
				// 衰减系数(0=无衰减,>0=逐渐减弱)
				float decay,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 波形
			static auto wave(
				entt::registry& registry,
				entt::entity entity,
				// 起始点
				sf::Vector2f start,
				// 运动方向(单位向量,不检查)
				sf::Vector2f direction,
				// 振幅(垂直于运动方向的摆动幅度)
				float amplitude,
				// 波长(摆动一个完整周期的距离)
				float wavelength,
				// 前进速度(像素/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 钟摆
			static auto swing(
				entt::registry& registry,
				entt::entity entity,
				// 支点(悬挂点)
				sf::Vector2f pivot,
				// 摆长
				float radius,
				// 起始角度(弧度)
				float start_angle,
				// 结束角度(弧度)
				float end_angle,
				// 摆动速度(弧度/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;
		};

		class Scale final
		{
		public:
			// 震荡
			static auto oscillator(
				entt::registry& registry,
				entt::entity entity,
				// 最小缩放
				sf::Vector2f min,
				// 最大缩放
				sf::Vector2f max,
				// 缩放速度(次/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 弹性
			static auto spring(
				entt::registry& registry,
				entt::entity entity,
				// 最小缩放
				sf::Vector2f min,
				// 最大缩放
				sf::Vector2f max,
				// 弹簧强度(像素/秒²)
				float strength,
				// 阻尼系数
				float damping,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 呼吸
			static auto breathing(
				entt::registry& registry,
				entt::entity entity,
				// 最小缩放(呼气)
				sf::Vector2f min,
				// 最大缩放(吸气)
				sf::Vector2f max,
				// 速度(周期/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 压扁拉伸
			static auto squash_stretch(
				entt::registry& registry,
				entt::entity entity,
				// 压扁系数(x放大倍数 = 1/√factor, y缩小倍数 = √factor)
				float squash_factory,
				// 恢复速度(像素/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 定向脉冲
			static auto directional_pulse(
				entt::registry& registry,
				entt::entity entity,
				// 脉冲中心
				sf::Vector2f center,
				// 脉冲方向(单位向量,不检查)
				sf::Vector2f direction,
				// 垂直(X)/水平(Y)方向缩放
				sf::Vector2f scale,
				// 脉冲速度(次/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 果冻
			static auto jelly(
				entt::registry& registry,
				entt::entity entity,
				// 目标缩放
				sf::Vector2f target_scale,
				// 过冲幅度(超出目标的百分百)
				float overshoot,
				// 震荡频率(赫兹)
				float frequency,
				// 阻尼系数
				float damping,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 弹性碰撞
			static auto elastic_hit(
				entt::registry& registry,
				entt::entity entity,
				// 压缩缩放(x<1, y<1 或 x<1, y>1)
				sf::Vector2f compress_scale,
				// 拉伸缩放
				sf::Vector2f stretch_scale,
				// 恢复速度
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 涟漪
			static auto ripple(
				entt::registry& registry,
				entt::entity entity,
				// 中心缩放
				sf::Vector2f center_scale,
				// 边缘缩放
				sf::Vector2f edge_scale,
				// 涟漪传播速度(次/秒)
				float speed,
				// 持续时间
				float duration
			) noexcept -> void;
		};

		class Color final
		{
		public:
			// 渐变
			static auto fade(
				entt::registry& registry,
				entt::entity entity,
				// 初始颜色
				sf::Color start,
				// 结束颜色
				sf::Color end,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// Alpha渐变
			static auto alpha_fade(
				entt::registry& registry,
				entt::entity entity,
				// 初始透明度
				float start,
				// 结束透明度
				float end,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 闪烁
			static auto flash(
				entt::registry& registry,
				entt::entity entity,
				// 基础颜色
				sf::Color base_color,
				// 闪烁颜色
				sf::Color flash_color,
				// 闪烁速度(次/秒)
				float speed,
				// 衰减系数(0=无衰减,>0=强度逐渐减弱)
				float decay,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 呼吸
			static auto breathing(
				entt::registry& registry,
				entt::entity entity,
				// 最小颜色(呼气)
				sf::Color min,
				// 最大颜色(吸气)
				sf::Color max,
				// 速度(周期/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 脉冲
			static auto pulse(
				entt::registry& registry,
				entt::entity entity,
				// 低谷颜色
				sf::Color base_color,
				// 峰值颜色
				sf::Color peak_color,
				// 脉冲速度(次/秒)
				float speed,
				// 峰值持续时间(秒)
				float peak_duration,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 彩虹循环
			static auto rainbow_cycle(
				entt::registry& registry,
				entt::entity entity,
				// 变化速度(周期/秒)
				float speed,
				// 饱和度
				float saturation,
				// 亮度
				float lightness,
				// 持续时间(秒)
				float duration
			) noexcept -> void;

			// 震荡
			static auto oscillator(
				entt::registry& registry,
				entt::entity entity,
				// 震荡颜色1
				sf::Color color1,
				// 震荡颜色2
				sf::Color color2,
				// 震荡速度(次/秒)
				float speed,
				// 持续时间(秒)
				float duration
			) noexcept -> void;
		};
	};
}
