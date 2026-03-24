// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <concepts>
#include <random>

namespace pd::utility
{
	class Random
	{
	public:
		using engine_type = std::mt19937;
		using seed_type = engine_type::result_type;

	private:
		engine_type engine_;

		[[nodiscard]] static auto engine() noexcept -> engine_type&
		{
			static Random instance{};
			return instance.engine_;
		}

	public:
		Random() noexcept
			: engine_{std::random_device{}()} {}

		static auto seed(const seed_type seed) noexcept -> void
		{
			engine().seed(seed);
		}

		[[nodiscard]] static auto operator()() noexcept -> std::ptrdiff_t
		{
			return engine()();
		}

		// 生成闭区间随机整数 [min_value, max_value]
		template<std::integral TInteger>
		[[nodiscard]] static auto int_inclusive(TInteger min_value, TInteger max_value) -> TInteger
		{
			return std::uniform_int_distribution<TInteger>{min_value, max_value}(engine());
		}

		// 生成闭区间随机浮点数 [min_value, max_value]
		template<std::floating_point TFloat>
		[[nodiscard]] static auto real_inclusive(TFloat min_value, TFloat max_value) -> TFloat
		{
			return std::uniform_real_distribution<TFloat>{min_value, max_value}(engine());
		}

		// 生成正态分布随机浮点数 N(mean, standard_deviation)
		template<std::floating_point TFloat>
		[[nodiscard]] static auto normal(TFloat mean, TFloat standard_deviation) -> TFloat
		{
			return std::normal_distribution<TFloat>{mean, standard_deviation}(engine());
		}

		// 百分比判定: 例如 roll_percent(20) 表示 20% 成功率
		[[nodiscard]] static auto roll_percent(const int percent) -> bool
		{
			if (percent <= 0)
			{
				return false;
			}
			if (percent >= 100)
			{
				return true;
			}

			return int_inclusive(0, 99) < percent;
		}

		// 概率判定: 概率范围 [0, 1]
		[[nodiscard]] static auto chance(const float probability) -> bool
		{
			if (probability <= 0.f)
			{
				return false;
			}
			if (probability >= 1.f)
			{
				return true;
			}

			return std::bernoulli_distribution{static_cast<double>(probability)}(engine());
		}
	};
}
