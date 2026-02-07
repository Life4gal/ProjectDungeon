// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <ciso646>
#include <type_traits>
#include <utility>

namespace pd
{
	template<typename>
	struct has_magic_enum_value : std::false_type {};

	template<typename T>
		requires requires { T::PD_MAGIC_ENUM_FLAG; }
	struct has_magic_enum_value<T> : std::true_type {};

	template<typename T>
		requires requires { T::PdMagicEnumFlag; }
	struct has_magic_enum_value<T> : std::true_type {};

	template<typename T>
		requires requires { T::pd_magic_enum_flag; }
	struct has_magic_enum_value<T> : std::true_type {};

	template<typename T>
	concept magic_enum_value_t = has_magic_enum_value<T>::value;

	template<typename>
	struct enum_is_flag : std::false_type {};

	template<magic_enum_value_t EnumType>
	struct enum_is_flag<EnumType> : std::true_type {};
}

// =============================================================================
// operator|

// flag | value => flag
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator|(const EnumType lhs, const ValueType rhs) noexcept -> EnumType //
	requires requires { std::to_underlying(lhs) | rhs; }
{
	return static_cast<EnumType>(std::to_underlying(lhs) | rhs);
}

// value | flag => value
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator|(const ValueType lhs, const EnumType rhs) noexcept -> ValueType //
	requires requires { lhs | std::to_underlying(rhs); }
{
	return static_cast<ValueType>(lhs | std::to_underlying(rhs));
}

// flag | flag => flag
template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator|(const EnumType lhs, const EnumType rhs) noexcept -> EnumType //
	requires requires { std::to_underlying(lhs) | std::to_underlying(rhs); }
{
	return static_cast<EnumType>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

// flag |= value => flag
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator|=(EnumType& lhs, const ValueType rhs) noexcept -> EnumType& //
	requires requires { lhs | rhs; }
{
	lhs = lhs | rhs;
	return lhs;
}

// value |= flag => value
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator|=(ValueType& lhs, const EnumType rhs) noexcept -> ValueType& //
	requires requires { lhs | rhs; }
{
	lhs = lhs | rhs;
	return lhs;
}

// flag |= flag => flag
template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator|=(EnumType& lhs, const EnumType rhs) noexcept -> EnumType& //
	requires requires { lhs | rhs; }
{
	lhs = lhs | rhs;
	return lhs;
}

// =============================================================================
// operator&

// flag & value => flag
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator&(const EnumType lhs, const ValueType rhs) noexcept -> EnumType //
	requires requires { std::to_underlying(lhs) & rhs; }
{
	return static_cast<EnumType>(std::to_underlying(lhs) & rhs);
}

// value & flag => value
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator&(const ValueType lhs, const EnumType rhs) noexcept -> ValueType //
	requires requires { lhs & std::to_underlying(rhs); }
{
	return static_cast<ValueType>(lhs & std::to_underlying(rhs));
}

// flag & flag => flag
template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator&(const EnumType lhs, const EnumType rhs) noexcept -> EnumType //
	requires requires { std::to_underlying(lhs) & std::to_underlying(rhs); }
{
	return static_cast<EnumType>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

// flag &= value => flag
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator&=(EnumType& lhs, const ValueType rhs) noexcept -> EnumType& //
	requires requires { lhs & rhs; }
{
	lhs = lhs & rhs;
	return lhs;
}

// value &= flag => value
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator&=(ValueType& lhs, const EnumType rhs) noexcept -> ValueType& //
	requires requires { lhs & rhs; }
{
	lhs = lhs & rhs;
	return lhs;
}

// flag &= flag => flag
template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator&=(EnumType& lhs, const EnumType rhs) noexcept -> EnumType& //
	requires requires { lhs & rhs; }
{
	lhs = lhs & rhs;
	return lhs;
}

// =============================================================================
// operator^

// flag ^ value => flag
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator^(const EnumType lhs, const ValueType rhs) noexcept -> EnumType //
	requires requires { std::to_underlying(lhs) ^ rhs; }
{
	return static_cast<EnumType>(std::to_underlying(lhs) ^ rhs);
}

// value ^ flag => value
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator^(const ValueType lhs, const EnumType rhs) noexcept -> ValueType //
	requires requires { lhs ^ std::to_underlying(rhs); }
{
	return static_cast<ValueType>(lhs ^ std::to_underlying(rhs));
}

// flag ^ flag => flag
template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator^(const EnumType lhs, const EnumType rhs) noexcept -> EnumType //
	requires requires { std::to_underlying(lhs) ^ std::to_underlying(rhs); }
{
	return static_cast<EnumType>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
}

// flag ^= value => flag
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator^=(EnumType& lhs, const ValueType rhs) noexcept -> EnumType& //
	requires requires { lhs ^ rhs; }
{
	lhs = lhs ^ rhs;
	return lhs;
}

// value ^= flag => value
template<typename EnumType, std::integral ValueType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator^=(ValueType& lhs, const EnumType rhs) noexcept -> ValueType& //
	requires requires { lhs ^ rhs; }
{
	lhs = lhs ^ rhs;
	return lhs;
}

// flag ^= flag => flag
template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
constexpr auto operator^=(EnumType& lhs, const EnumType rhs) noexcept -> EnumType& //
	requires requires { lhs ^ rhs; }
{
	lhs = lhs ^ rhs;
	return lhs;
}

// =============================================================================
// operator~

template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator~(const EnumType e) noexcept -> EnumType //
	requires requires { ~std::to_underlying(e); }
{
	return static_cast<EnumType>(~std::to_underlying(e));
}

// =============================================================================
// operator!

template<typename EnumType>
	requires std::is_scoped_enum_v<EnumType> and pd::enum_is_flag<EnumType>::value
[[nodiscard]] constexpr auto operator!(const EnumType e) noexcept -> bool //
	requires requires { !std::to_underlying(e); }
{
	return !std::to_underlying(e);
}
