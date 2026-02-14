// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <asset/asset_loader.hpp>

#include <SFML/Audio/Music.hpp>

namespace pd::asset
{
	namespace music_detail
	{
		class Loader final
		{
		public:
			using result_type = std::shared_ptr<sf::Music>;

			[[nodiscard]] static auto operator()(const path_type& path) noexcept -> result_type;
		};
	}

	class MusicLoader final : public AssetLoader<music_detail::Loader> {};
}
