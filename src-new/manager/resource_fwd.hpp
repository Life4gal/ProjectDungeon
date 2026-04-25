// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/resource/resource.hpp>

namespace sf
{
	class Font;
	class Texture;
	class SoundBuffer;
	class Music;
}

namespace pd::manager
{
	namespace resource_detail
	{
		class InvalidHandler final
		{
		public:
			template<typename T>
			[[nodiscard]] constexpr explicit(false) operator entt::resource<T>() const noexcept
			{
				return {};
			}

			template<typename T>
			[[nodiscard]] constexpr auto operator==(const entt::resource<T>& handler) const noexcept -> bool
			{
				return handler == entt::resource<T>{};
			}
		};
	}

	// FIXME: entt::resource不支持entt::resource<const T>{entt::resource<T>{}}

	using font_handler = entt::resource</*const*/ sf::Font>;
	using texture_handler = entt::resource</*const*/ sf::Texture>;
	using sound_handler = entt::resource</*const*/ sf::SoundBuffer>;
	// music需要暂停,所以不是const
	using music_handler = entt::resource<sf::Music>;

	constexpr resource_detail::InvalidHandler InvalidHandler{};
}
