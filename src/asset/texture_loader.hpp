// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <asset/asset_loader.hpp>

#include <game/constants.hpp>

#include <SFML/Graphics/Texture.hpp>

namespace pd::asset
{
	namespace texture_detail
	{
		class Loader final
		{
		public:
			using result_type = std::shared_ptr<sf::Texture>;

			[[nodiscard]] static auto operator()(const path_type& path) noexcept -> result_type;
		};
	}

	class TextureLoader final : public AssetLoader<texture_detail::Loader>
	{
	public:
		// ctx::Map需要此name来创建/获取正确的map
		constexpr static auto name = Constant::asset_texture;
	};
}
