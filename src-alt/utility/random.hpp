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

		using result_type = engine_type::result_type;

	private:
		engine_type engine_;

	public:
		Random() noexcept
			: engine_{std::random_device{}()} {}

		[[nodiscard]] constexpr static auto min() noexcept -> result_type
		{
			return engine_type::min();
		}

		[[nodiscard]] constexpr static auto max() noexcept -> result_type
		{
			return engine_type::max();
		}

		auto seed(const result_type seed) noexcept -> void
		{
			engine_.seed(seed);
		}

		[[nodiscard]] auto operator()() noexcept -> result_type
		{
			return engine_();
		}

		// 生成闭区间随机整数 [min_value, max_value]
		template<std::integral Integer>
		[[nodiscard]] auto int_inclusive(const Integer min_value, const std::type_identity_t<Integer> max_value) noexcept -> Integer
		{
			return std::uniform_int_distribution<Integer>{min_value, max_value}(engine_);
		}

		// 生成闭区间随机浮点数 [min_value, max_value]
		template<std::floating_point Float>
		[[nodiscard]] auto real_inclusive(const Float min_value, const std::type_identity_t<Float> max_value) noexcept -> Float
		{
			return std::uniform_real_distribution<Float>{min_value, max_value}(engine_);
		}

		// 生成正态分布随机浮点数 N(mean, standard_deviation)
		template<std::floating_point Float>
		[[nodiscard]] auto normal(const Float mean, const std::type_identity_t<Float> standard_deviation) noexcept -> Float
		{
			return std::normal_distribution<Float>{mean, standard_deviation}(engine_);
		}

		// 百分比判定: 例如 roll_percent(20) 表示 20% 成功率
		[[nodiscard]] auto roll_percent(const int percent) noexcept -> bool
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
		[[nodiscard]] auto chance(const float probability) noexcept -> bool
		{
			if (probability <= 0.f)
			{
				return false;
			}
			if (probability >= 1.f)
			{
				return true;
			}

			return std::bernoulli_distribution{static_cast<double>(probability)}(engine_);
		}
	};
}
