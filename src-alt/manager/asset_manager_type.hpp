// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/core/fwd.hpp>
#include <entt/entity/entity.hpp>
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
	enum class AssetId : entt::id_type {};

	constexpr static auto invalid_asset_id = AssetId{entt::null};

	using font_type = entt::resource<const sf::Font>;
	using texture_type = entt::resource<const sf::Texture>;
	using sound_type = entt::resource<const sf::SoundBuffer>;
	// music需要暂停,所有不是const
	using music_type = entt::resource<sf::Music>;
}
