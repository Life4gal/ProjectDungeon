// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility/random.hpp>

namespace pd::manager
{
	class Random final
	{
		using random_type = utility::Random;

	public:
		using engine_type = random_type::engine_type;
		using seed_type = random_type::seed_type;

	private:
		random_type random_;

	public:
		// 这个接口公开仅为了Game::debug_random_
		[[nodiscard]] static auto instance() noexcept -> Random&;

	private:
		[[nodiscard]] static auto engine() noexcept -> random_type&
		{
			return instance().random_;
		}

	public:
		static auto seed(const seed_type seed) noexcept -> void
		{
			engine().seed(seed);
		}

		[[nodiscard]] static auto operator()() noexcept -> std::ptrdiff_t
		{
			return engine()();
		}

		// 生成闭区间随机整数 [min_value, max_value]
		template<std::integral Integer>
		[[nodiscard]] static auto int_inclusive(const Integer min_value, const std::type_identity_t<Integer> max_value) noexcept -> Integer
		{
			return engine().int_inclusive(min_value, max_value);
		}

		// 生成闭区间随机浮点数 [min_value, max_value]
		template<std::floating_point Float>
		[[nodiscard]] static auto real_inclusive(const Float min_value, const std::type_identity_t<Float> max_value) noexcept -> Float
		{
			return engine().real_inclusive(min_value, max_value);
		}

		// 生成正态分布随机浮点数 N(mean, standard_deviation)
		template<std::floating_point Float>
		[[nodiscard]] static auto normal(const Float mean, const std::type_identity_t<Float> standard_deviation) noexcept -> Float
		{
			return engine().normal(mean, standard_deviation);
		}

		// 百分比判定: 例如 roll_percent(20) 表示 20% 成功率
		[[nodiscard]] static auto roll_percent(const int percent) noexcept -> bool
		{
			return engine().roll_percent(percent);
		}

		// 概率判定: 概率范围 [0, 1]
		[[nodiscard]] static auto chance(const float probability) noexcept -> bool
		{
			return engine().chance(probability);
		}
	};
}
