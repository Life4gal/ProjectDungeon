// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/asset_fwd.hpp>

#include <utility/resource_cache.hpp>

namespace pd::manager
{
	namespace detail
	{
		template<typename Derived, typename Loader>
		class Asset
		{
			using loader_type = Loader;
			using cache_type = utility::ResourceCache<loader_type>;

		public:
			using reference = cache_type::reference;
			using const_reference = cache_type::const_reference;

			using size_type = cache_type::size_type;

		private:
			cache_type cache_;

			[[nodiscard]] static auto cache() noexcept -> cache_type&
			{
				return Derived::instance().cache_;
			}

		public:
			[[nodiscard]] static auto load(const std::string_view path) noexcept -> AssetId
			{
				return cache().load(path);
			}

			[[nodiscard]] static auto force_load(const std::string_view path) -> AssetId
			{
				return cache().force_load(path);
			}

			[[nodiscard]] static auto get(const AssetId id) noexcept -> reference
			{
				return cache().get(id);
			}

			[[nodiscard]] static auto get(const std::string_view path) noexcept -> reference
			{
				return cache().get(path);
			}

			[[nodiscard]] static auto empty() noexcept -> bool
			{
				return cache().empty();
			}

			[[nodiscard]] static auto size() noexcept -> size_type
			{
				return cache().size();
			}

			[[nodiscard]] static auto contains(const AssetId id) noexcept -> bool
			{
				return cache().contains(id);
			}

			[[nodiscard]] static auto contains(const std::string_view path) noexcept -> bool
			{
				return cache().contains(path);
			}

			static auto unload(const AssetId id) noexcept -> bool
			{
				return cache().erase(id);
			}

			static auto unload(const std::string_view path) noexcept -> bool
			{
				return cache().erase(path);
			}

			static auto clear() noexcept -> void
			{
				cache().clear();
			}
		};

		class FontLoader final
		{
		public:
			using result_type = std::shared_ptr<std::remove_const_t<font_type::element_type>>;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> result_type;
		};

		class TextureLoader final
		{
		public:
			using result_type = std::shared_ptr<std::remove_const_t<texture_type::element_type>>;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> result_type;
		};

		class SoundLoader final
		{
		public:
			using result_type = std::shared_ptr<std::remove_const_t<sound_type::element_type>>;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> result_type;
		};

		class MusicLoader final
		{
		public:
			using result_type = std::shared_ptr<std::remove_const_t<music_type::element_type>>;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> result_type;
		};
	}

	class Font final : public detail::Asset<Font, detail::FontLoader>
	{
	public:
		// 这个接口公开仅为了Game::debug_font_
		[[nodiscard]] static auto instance() noexcept -> Font&;
	};

	class Texture final : public detail::Asset<Texture, detail::TextureLoader>
	{
	public:
		// 这个接口公开仅为了Game::debug_texture_
		[[nodiscard]] static auto instance() noexcept -> Texture&;
	};

	class Sound final : public detail::Asset<Sound, detail::SoundLoader>
	{
	public:
		// 这个接口公开仅为了Game::debug_sound_
		[[nodiscard]] static auto instance() noexcept -> Sound&;

		// 播放一个音效
		// false:
		//  -指定的音效未加载
		//  -指定的音效正在播放
		//  -不存在空闲音效通道
		static auto play(AssetId id) noexcept -> bool;

		// 播放一个音效
		// false:
		//  -指定的音效未加载
		//  -指定的音效正在播放
		//  -不存在空闲音效通道
		static auto play(std::string_view path) noexcept -> bool;

		// 播放一个音效,如果指定音效正在播放则跳过
		// false:
		//  -指定的音效未加载
		//  -不存在空闲音效通道
		static auto play_unique(AssetId id) noexcept -> bool;

		// 播放一个音效,如果指定音效正在播放则跳过
		// false:
		//  -指定的音效未加载
		//  -不存在空闲音效通道
		static auto play_unique(std::string_view path) noexcept -> bool;
	};

	class Music final : public detail::Asset<Music, detail::MusicLoader>
	{
	public:
		// 这个接口公开仅为了Game::debug_music_
		[[nodiscard]] static auto instance() noexcept -> Music&;

		// 播放一个音乐,如果音乐正在播放则跳过
		// false:
		//  -指定的音乐未加载
		//  -指定的音乐正在播放
		static auto play(AssetId id, bool loop = true) noexcept -> bool;

		// 播放一个音乐,如果音乐正在播放则跳过
		// false:
		//  -指定的音乐未加载
		//  -指定的音乐正在播放
		static auto play(std::string_view path, bool loop = true) noexcept -> bool;

		// 播放一个音乐,如果音乐正在播放则重启
		// false:
		//  -指定的音乐未加载
		static auto play_or_restart(AssetId id, bool loop = true) noexcept -> bool;

		// 播放一个音乐,如果音乐正在播放则重启
		// false:
		//  -指定的音乐未加载
		static auto play_or_restart(std::string_view path, bool loop = true) noexcept -> bool;

		// 停止一个音乐
		// false:
		//  -指定的音乐未加载
		// 指定的音乐未播放也视为停止成功
		static auto stop(AssetId id) noexcept -> bool;

		// 停止一个音乐
		// false:
		//  -指定的音乐未加载
		// 指定的音乐未播放也视为停止成功
		static auto stop(std::string_view path) noexcept -> bool;

		// 停止当前音乐
		// false:
		//  -未指定音乐播放
		static auto stop_all() noexcept -> bool;
	};
}
