// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

namespace pd::utility
{
	template<typename T, std::size_t Rows, std::size_t Columns>
	class FixedMatrix final
	{
		using matrix_type = std::array<T, Rows * Columns>;

	public:
		using value_type = matrix_type::value_type;
		using size_type = matrix_type::size_type;
		using difference_type = matrix_type::difference_type;
		using pointer = matrix_type::pointer;
		using const_pointer = matrix_type::const_pointer;
		using reference = matrix_type::reference;
		using const_reference = matrix_type::const_reference;

		using iterator = matrix_type::iterator;
		using const_iterator = matrix_type::const_iterator;

		using reverse_iterator = matrix_type::reverse_iterator;
		using const_reverse_iterator = matrix_type::const_reverse_iterator;

		constexpr static auto rows = Rows;
		constexpr static auto columns = Columns;

	private:
		matrix_type matrix_;

	public:
		constexpr auto fill(const_reference value) noexcept -> void
		{
			matrix_.fill(value);
		}

		constexpr auto swap(FixedMatrix& other) noexcept -> void
		{
			matrix_.swap(other.matrix_);
		}

		[[nodiscard]] constexpr auto begin() noexcept -> iterator
		{
			return matrix_.begin();
		}

		[[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
		{
			return matrix_.begin();
		}

		[[nodiscard]] constexpr auto end() noexcept -> iterator
		{
			return matrix_.end();
		}

		[[nodiscard]] constexpr auto end() const noexcept -> const_iterator
		{
			return matrix_.end();
		}

		[[nodiscard]] constexpr auto rbegin() noexcept -> reverse_iterator
		{
			return matrix_.rbegin();
		}

		[[nodiscard]] constexpr auto rbegin() const noexcept -> const_reverse_iterator
		{
			return matrix_.rbegin();
		}

		[[nodiscard]] constexpr auto rend() noexcept -> reverse_iterator
		{
			return matrix_.rend();
		}

		[[nodiscard]] constexpr auto rend() const noexcept -> const_reverse_iterator
		{
			return matrix_.rend();
		}

		[[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator
		{
			return matrix_.cbegin();
		}

		[[nodiscard]] constexpr auto cend() const noexcept -> const_iterator
		{
			return matrix_.cend();
		}

		[[nodiscard]] constexpr auto crbegin() const noexcept -> const_reverse_iterator
		{
			return matrix_.crbegin();
		}

		[[nodiscard]] constexpr auto crend() const noexcept -> const_reverse_iterator
		{
			return matrix_.crend();
		}

		[[nodiscard]] constexpr auto size() const noexcept -> size_type
		{
			return matrix_.size();
		}

		[[nodiscard]] constexpr auto max_size() const noexcept -> size_type
		{
			return matrix_.max_size();
		}

		[[nodiscard]] constexpr auto at(const size_type row, const size_type column) -> reference
		{
			return matrix_.at(row * columns + column);
		}

		[[nodiscard]] constexpr auto at(const size_type row, const size_type column) const -> const_reference
		{
			return matrix_.at(row * columns + column);
		}

		[[nodiscard]] constexpr auto operator[](const size_type row, const size_type column) noexcept -> reference
		{
			return matrix_[row * columns + column];
		}

		[[nodiscard]] constexpr auto operator[](const size_type row, const size_type column) const noexcept -> const_reference
		{
			return matrix_[row * columns + column];
		}

		[[nodiscard]] constexpr auto front() noexcept -> reference
		{
			return matrix_.front();
		}

		[[nodiscard]] constexpr auto front() const noexcept -> const_reference
		{
			return matrix_.front();
		}

		[[nodiscard]] constexpr auto back() noexcept -> reference
		{
			return matrix_.back();
		}

		[[nodiscard]] constexpr auto back() const noexcept -> const_reference
		{
			return matrix_.back();
		}

		[[nodiscard]] constexpr auto data() noexcept -> pointer
		{
			return matrix_.data();
		}

		[[nodiscard]] constexpr auto data() const noexcept -> const_pointer
		{
			return matrix_.data();
		}
	};

	// 不提供推导指引,因为必须要指定行列数
}
