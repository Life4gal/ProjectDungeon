// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/asset_fwd.hpp>

#include <entt/core/hashed_string.hpp>
#include <entt/resource/cache.hpp>

namespace pd::manager
{
	namespace detail
	{
		[[nodiscard]] constexpr auto to_id(const AssetId id) noexcept -> entt::id_type
		{
			return std::to_underlying(id);
		}

		[[nodiscard]] constexpr auto from_id(const entt::id_type id) noexcept -> AssetId
		{
			return static_cast<AssetId>(id);
		}

		[[nodiscard]] constexpr auto make_id(const std::string_view path) noexcept -> entt::id_type
		{
			return entt::hashed_string{path.data(), path.size()};
		}

		template<typename Loader>
		class Asset
		{
			using loader_type = Loader;
			using element_type = Loader::result_type::element_type;
			using cache_type = entt::resource_cache<element_type, loader_type>;

		public:
			using reference = entt::resource<element_type>;
			using const_reference = entt::resource<const element_type>;

			using size_type = cache_type::size_type;

		private:
			inline static cache_type cache_;

		public:
			// [[nodiscard]] static auto instance() noexcept -> cache_type&
			// {
			// 	return cache_;
			// }

			[[nodiscard]] static auto load(const std::string_view path) noexcept -> AssetId
			{
				const auto id = make_id(path);
				[[maybe_unused]] const auto [it, loaded] = cache_.load(id, path);

				return from_id(id);
			}

			[[nodiscard]] static auto force_load(const std::string_view path) -> AssetId
			{
				const auto id = make_id(path);
				[[maybe_unused]] const auto [it, loaded] = cache_.force_load(id, path);

				return from_id(id);
			}

		private:
			[[nodiscard]] static auto get(const entt::id_type id) noexcept -> reference
			{
				return cache_[id];
			}

		public:
			[[nodiscard]] static auto get(const AssetId id) noexcept -> reference
			{
				return get(to_id(id));
			}

			[[nodiscard]] static auto get(const std::string_view path) noexcept -> reference
			{
				const auto id = make_id(path);

				return get(id);
			}

			[[nodiscard]] static auto empty() noexcept -> bool
			{
				return cache_.empty();
			}

			[[nodiscard]] static auto size() noexcept -> size_type
			{
				return cache_.size();
			}

		private:
			[[nodiscard]] static auto contains(const entt::id_type id) noexcept -> bool
			{
				return cache_.contains(id);
			}

		public:
			[[nodiscard]] static auto contains(const AssetId id) noexcept -> bool
			{
				return contains(to_id(id));
			}

			[[nodiscard]] static auto contains(const std::string_view path) noexcept -> bool
			{
				const auto id = make_id(path);

				return contains(id);
			}

		private:
			static auto erase(const entt::id_type id) noexcept -> bool
			{
				return cache_.erase(id) != 0;
			}

		public:
			static auto unload(const AssetId id) noexcept -> bool
			{
				return erase(to_id(id));
			}

			static auto unload(const std::string_view path) noexcept -> bool
			{
				const auto id = make_id(path);

				return erase(id);
			}

			static auto clear() noexcept -> void
			{
				cache_.clear();
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

	class Font final : public detail::Asset<detail::FontLoader> {};

	class Texture final : public detail::Asset<detail::TextureLoader> {};

	class Sound final : public detail::Asset<detail::SoundLoader>
	{
	public:
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

	class Music final : public detail::Asset<detail::MusicLoader>
	{
	public:
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
