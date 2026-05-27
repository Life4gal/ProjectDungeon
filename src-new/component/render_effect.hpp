// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace pd::component::render_effect
{
	// 精灵特效
	namespace sprite
	{
		// 由特效产生的额外偏移
		// 实际渲染位置 = transform::Position + position
		// 该组件允许我们*渲染*的位置与*实际*的位置不同
		// 我们可以基于此做到一些动画效果(而不影响实体实际的位置)
		class Position final
		{
		public:
			sf::Vector2f extra;
		};

		// 由特效产生的额外缩放
		// 实际渲染缩放 = transform::Scale * scale
		// 该组件允许我们*渲染*的大小与*实际*的大小不同
		// 我们可以基于此做到一些动画效果(而不影响实体的实际大小)
		class Scale final
		{
		public:
			sf::Vector2f extra;
		};

		// 由特效产生的额外旋转
		// 实际渲染的旋转 = transform::Rotation + rotation
		// 该组件允许我们*渲染*的旋转与*实际*的旋转不同
		// 我们可以基于此做到一些动画效果(而不影响实体的实际旋转)
		class Rotation final
		{
		public:
			sf::Angle extra;
		};

		// 渲染的颜色
		// 默认为白色
		class Color final
		{
		public:
			sf::Color color;
		};

		// ============================================

		// 动画可以叠加,所以每种动画有自己的计时器
		// TODO: 当前特效仅允许每个种类(position/scale/color)同时一种生效

		// ============================================
		// 位置效果
		// ============================================

		namespace position
		{
			// 线性
			class Linear final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 起始位置
				sf::Vector2f start;
				// 终点位置
				sf::Vector2f end;
				// 速度(像素/秒)
				float speed;

				// =======================
				// 变量(常量)
				// =======================

				// 两点X/Y轴距离
				sf::Vector2f offset;
				// 两点之间距离
				float distance;

				// =======================
				// 状态
				// =======================

				// 已移动距离
				float traveled;
			};

			// 震荡
			class Oscillator final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 震荡点1
				sf::Vector2f p1;
				// 震荡点2
				sf::Vector2f p2;
				// 速度(像素/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 两点X/Y轴距离
				sf::Vector2f offset;
				// 两点之间距离
				float distance;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 弹簧
			class Spring final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 起始位置
				sf::Vector2f start;
				// 目标位置
				sf::Vector2f target;
				// 弹簧强度(像素/秒²)
				float strength;
				// 阻尼系数
				float damping;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 当前速度
				// = {0,0}
				sf::Vector2f velocity;
				// 当前位置(不信任Position组件提供的位置)
				// = start
				sf::Vector2f position;
				// 已持续时间(秒)
				float elapsed;
			};

			// 路径
			class Path final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 所有路径点
				std::vector<sf::Vector2f> waypoints;
				// 速度(像素/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 共有waypoints.size() - 1个segment
				// std::uint32_t segment_count = waypoints.size() - 1;

				// 各个路径点X/Y轴距离(size = segment_count)
				std::vector<sf::Vector2f> offsets;
				// 各个路径点之间距离(size = segment_count)
				std::vector<float> distances;

				// =======================
				// 状态
				// =======================

				// 当前路径索引
				// [0, segment_count)
				// = 0
				std::uint32_t current_segment;
				// 当前路径已移动距离
				// = 0
				float current_segment_traveled;
				// 当前位置(不信任Position组件提供的位置)
				// = waypoints[0]
				sf::Vector2f position;
				// 已持续时间(秒)
				float elapsed;
			};

			// 轨道环绕
			class Orbit final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 环绕中心
				sf::Vector2f center;
				// 半径
				float radius;
				// 角速度(弧度/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 当前角度
				// = 0
				float angle;
				// 已持续时间(秒)
				float elapsed;
			};

			// 抖动/摇晃
			class Shake final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 中心点
				sf::Vector2f center;
				// 抖动强度(像素)
				float intensity;
				// 抖动频率(次/秒)
				float frequency;
				// 衰减系数(0=无衰减,>0=逐渐减弱)
				float decay;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 当前阶段
				// = 0
				float phase;
				// 已持续时间(秒)
				float elapsed;
			};

			// 波形
			class Wave final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 起始点
				sf::Vector2f start;
				// 运动方向(单位向量,不检查)
				sf::Vector2f direction;
				// 振幅(垂直于运动方向的摆动幅度)
				float amplitude;
				// 波长(摆动一个完整周期的距离)
				float wavelength;
				// 前进速度(像素/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 垂直方向
				// = {-direction.y, direction.x}
				sf::Vector2f perpendicular_direction;

				// =======================
				// 状态
				// =======================

				// 已移动距离
				float traveled;
				// 已持续时间(秒)
				float elapsed;
			};

			// 钟摆
			class Swing final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 支点(悬挂点)
				sf::Vector2f pivot;
				// 摆长
				float radius;
				// 起始角度(弧度)
				float start_angle;
				// 结束角度(弧度)
				float end_angle;
				// 摆动速度(弧度/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 摆角差
				// = abs(end_angle - start_angle)
				float angle_diff;
				// 总摆角
				// = angle_diff * 2
				float angle_total;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			class Effect final : public std::variant<
						Linear,
						Oscillator,
						Spring,
						Path,
						Orbit,
						Shake,
						Wave,
						Swing
					> {};
		}

		// ============================================
		// 大小效果
		// ============================================

		namespace scale
		{
			// 震荡
			class Oscillator final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 最小缩放
				sf::Vector2f min;
				// 最大缩放
				sf::Vector2f max;
				// 缩放速度(次/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 最小/最大缩放差值
				// = max - min
				sf::Vector2f diff;
				// 摇摆区间
				// = diff * 2
				sf::Vector2f swing;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 弹性
			class Spring final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 最小缩放
				sf::Vector2f min;
				// 最大缩放
				sf::Vector2f max;
				// 弹簧强度(像素/秒²)
				float strength;
				// 阻尼系数
				float damping;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 当前速度
				// = {0,0}
				sf::Vector2f velocity;
				// 当前缩放(不信任Scale组件提供的缩放)
				// = min
				sf::Vector2f scale;
				// 已持续时间(秒)
				float elapsed;
			};

			// 呼吸
			class Breathing
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 最小缩放(呼气)
				sf::Vector2f min;
				// 最大缩放(吸气)
				sf::Vector2f max;
				// 速度(周期/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 最小/最大缩放差值
				// = max - min
				sf::Vector2f diff;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 压扁拉伸
			class SquashStretch final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 压扁系数(x放大倍数 = 1/√factor, y缩小倍数 = √factor)
				float squash_factory;
				// 恢复速度(像素/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 定向脉冲
			class DirectionalPulse final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 脉冲中心
				sf::Vector2f center;
				// 脉冲方向(单位向量,不检查)
				sf::Vector2f direction;
				// 垂直(X)/水平(Y)方向缩放
				sf::Vector2f scale;
				// 脉冲速度(次/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 果冻
			class Jelly final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 目标缩放
				sf::Vector2f target_scale;
				// 过冲幅度(超出目标的百分百)
				float overshoot;
				// 震荡频率(赫兹)
				float frequency;
				// 阻尼系数
				float damping;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 当前速度
				// = {0,0}
				sf::Vector2f velocity;
				// 当前缩放(不信任Scale组件提供的缩放)
				// = target_scale - target_scale * overshoot
				sf::Vector2f current_scale;
				// 已持续时间(秒)
				float elapsed;
			};

			// 弹性碰撞
			class ElasticHit final
			{
			public:
				enum class Phase : std::uint8_t
				{
					COMPRESS,
					STRETCH,
					RESUME,
				};

				// =======================
				// 输入参数
				// =======================

				// 压缩缩放(x<1, y<1 或 x<1, y>1)
				sf::Vector2f compress_scale;
				// 拉伸缩放
				sf::Vector2f stretch_scale;
				// 恢复速度
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 每个阶段耗时(平均分成3份)
				// = duration / 3
				float phase_duration;
				// 压缩阶段缩放差
				// = compress_scale - {1,1}
				sf::Vector2f compress_phase_diff;
				// 拉伸阶段缩放差
				// = stretch_scale - compress_scale
				sf::Vector2f stretch_phase_diff;
				// 恢复阶段缩放差
				// = {1,1} - stretch_scale
				sf::Vector2f resume_phase_diff;

				// =======================
				// 状态
				// =======================

				// 当前阶段
				// = COMPRESS
				Phase phase;
				// 当前阶段已持续时间(秒)
				// = 0
				float phase_elapsed;

				// 已持续时间(秒)
				float elapsed;
			};

			// 涟漪
			class Ripple final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 中心缩放
				sf::Vector2f center_scale;
				// 边缘缩放
				sf::Vector2f edge_scale;
				// 涟漪传播速度(次/秒)
				float speed;
				// 持续时间
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 中心与边缘缩放差
				// = edge_scale - center_scale
				sf::Vector2f scale_diff;

				// =======================
				// 状态
				// =======================

				// 当前涟漪半径(0~1)
				// = 0
				float radius;
				// 已持续时间(秒)
				float elapsed;
			};

			class Effect final : public std::variant<
						Oscillator,
						Spring,
						Breathing,
						SquashStretch,
						DirectionalPulse,
						Jelly,
						ElasticHit,
						Ripple
					> {};
		}

		// ============================================
		// 颜色效果
		// ============================================

		namespace color
		{
			// 渐变
			class Fade final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 初始颜色
				sf::Color start;
				// 结束颜色
				sf::Color end;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 颜色差
				// = end - start
				sf::Color diff;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// Alpha渐变
			class AlphaFade final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 初始透明度
				float start;
				// 结束透明度
				float end;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 透明度差
				// = end - start
				float diff;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 闪烁
			class Flash final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 基础颜色
				sf::Color base_color;
				// 闪烁颜色
				sf::Color flash_color;
				// 闪烁速度(次/秒)
				float speed;
				// 衰减系数(0=无衰减,>0=强度逐渐减弱)
				float decay;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 颜色差
				// = flash_color - base_color
				sf::Color diff;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 呼吸
			class Breathing final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 最小颜色(呼气)
				sf::Color min;
				// 最大颜色(吸气)
				sf::Color max;
				// 速度(周期/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 颜色差
				// = max - min
				sf::Color diff;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 脉冲
			class Pulse final
			{
			public:
				// =======================
				// 输入参数
				// =======================

				// 低谷颜色
				sf::Color base_color;
				// 峰值颜色
				sf::Color peak_color;
				// 脉冲速度(次/秒)
				float speed;
				// 峰值持续时间(秒)
				float peak_duration;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 颜色差
				// = peak_color - base_color
				sf::Color diff;
				// 每个脉冲周期持续时间
				// = 1 / speed
				float cycle_duration;
				// 每个脉冲周期上升持续时间(上升和下降对半分时间)
				// = (cycle_duration - peak_duration) / 2
				float raise_duration;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 彩虹循环
			class RainbowCycle final
			{
			public:
				// 变化速度(周期/秒)
				float speed;
				// 饱和度
				float saturation;
				// 亮度
				float lightness;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			// 震荡
			class Oscillator final
			{
			public:
				// 震荡颜色1
				sf::Color color1;
				// 震荡颜色2
				sf::Color color2;
				// 震荡速度(次/秒)
				float speed;
				// 持续时间(秒)
				float duration;

				// =======================
				// 变量(常量)
				// =======================

				// 颜色差
				// = color2 - color1
				sf::Color diff;

				// =======================
				// 状态
				// =======================

				// 已持续时间(秒)
				float elapsed;
			};

			class Effect final : public std::variant<
						Fade,
						AlphaFade,
						Flash,
						Breathing,
						Pulse,
						RainbowCycle,
						Oscillator
					> {};
		}
	}

	// 其他特效?
}
