// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <asset/font_loader.hpp>
#include <asset/texture_loader.hpp>
#include <asset/sound_loader.hpp>
#include <asset/music_loader.hpp>

#include <ctx/map.hpp>

#include <entt/entity/registry.hpp>

namespace pd::ctx
{
	namespace asset_loader_detail
	{
		template<typename Loader>
		class AssetLoader
		{
			using loader_type = Loader;

		public:
			using result_type = loader_type::result_type;

			static auto emplace(entt::registry& registry) noexcept -> loader_type&
			{
				return registry.ctx().emplace<loader_type>();
			}

			[[nodiscard]] static auto get(entt::registry& registry) noexcept -> loader_type&
			{
				return registry.ctx().get<loader_type>();
			}

			static auto load(entt::registry& registry) noexcept -> void
			{
				const auto& map = Map<loader_type>::get(registry);

				return get(registry).load(map);
			}

			[[nodiscard]] static auto get(entt::registry& registry, const entt::id_type id) noexcept -> result_type
			{
				return get(registry).get(id);
			}
		};
	}

	class FontLoader final : public asset_loader_detail::AssetLoader<asset::FontLoader> {};

	class TextureLoader final : public asset_loader_detail::AssetLoader<asset::TextureLoader> {};

	class SoundLoader final : public asset_loader_detail::AssetLoader<asset::SoundLoader> {};

	class MusicLoader final : public asset_loader_detail::AssetLoader<asset::MusicLoader> {};
}
