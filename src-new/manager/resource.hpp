// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <unordered_map>

#include <manager/resource_fwd.hpp>

#include <entt/core/hashed_string.hpp>
#include <entt/resource/cache.hpp>

namespace pd::manager
{
	namespace resource_detail
	{
		[[nodiscard]] constexpr auto to_id(const std::string_view path) noexcept -> entt::id_type
		{
			return entt::hashed_string{path.data(), path.size()};
		}

		[[nodiscard]] inline auto to_id(const std::filesystem::path& path) noexcept -> entt::id_type
		{
			const auto& native = path.native();
			return entt::hashed_wstring{native.c_str(), native.size()};
		}

		template<typename Loader>
		class ResourceManager
		{
			using loader_type = Loader;
			using element_type = loader_type::element_type;
			using result_type = loader_type::result_type;

			using cache_type = entt::resource_cache<element_type, loader_type>;

		public:
			using handler_type = result_type;

		private:
			struct handler_hash
			{
				[[nodiscard]] static auto operator()(const handler_type& handler) noexcept -> std::size_t
				{
					return std::hash<const element_type*>{}(handler.operator->());
				}
			};

			inline static cache_type cache_;
			inline static std::unordered_map<handler_type, std::string, handler_hash> paths_;

		public:
			[[nodiscard]] static auto load(const std::string_view path) noexcept -> handler_type
			{
				const auto id = to_id(path);
				const auto [it, loaded] = cache_.load(id, path);
				auto handler = handler_type{it->second};

				if (loaded)
				{
					paths_.emplace(handler, path);
				}

				return handler;
			}

			[[nodiscard]] static auto load(const std::filesystem::path& path) noexcept -> handler_type
			{
				const auto id = to_id(path);
				const auto [it, loaded] = cache_.load(id, path);
				auto handler = handler_type{it->second};

				if (loaded)
				{
					paths_.emplace(handler, path.string());
				}

				return handler;
			}

			[[nodiscard]] static auto contains(const std::string_view path) noexcept -> bool
			{
				const auto id = to_id(path);

				return cache_.contains(id);
			}

			[[nodiscard]] static auto contains(const std::filesystem::path& path) noexcept -> bool
			{
				const auto id = to_id(path);

				return cache_.contains(id);
			}

			static auto unload(const std::string_view path) noexcept -> bool
			{
				const auto id = to_id(path);
				const auto handler = cache_[id];

				paths_.erase(handler);
				return cache_.erase(id) != 0;
			}

			static auto unload(const std::filesystem::path& path) noexcept -> bool
			{
				const auto id = to_id(path);
				const auto handler = cache_[id];

				paths_.erase(handler);
				return cache_.erase(id) != 0;
			}

			static auto clear() noexcept -> void
			{
				cache_.clear();
				paths_.clear();
			}

			[[nodiscard]] static auto path_of(const handler_type& handler) noexcept -> std::string_view
			{
				if (const auto it = paths_.find(handler);
					it != paths_.end())
				{
					return it->second;
				}

				return {};
			}
		};

		class FontLoader final
		{
		public:
			// ResourceManager内部entt::resource_cache保存的类型
			using element_type = std::remove_const_t<font_handler::element_type>;
			// ResourceManager返回给用户的类型
			using result_type = font_handler;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> entt::resource<element_type>;
			[[nodiscard]] auto operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>;
		};

		class TextureLoader final
		{
		public:
			// ResourceManager内部entt::resource_cache保存的类型
			using element_type = std::remove_const_t<texture_handler::element_type>;
			// ResourceManager返回给用户的类型
			using result_type = texture_handler;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> entt::resource<element_type>;
			[[nodiscard]] auto operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>;
		};

		class SoundLoader final
		{
		public:
			// ResourceManager内部entt::resource_cache保存的类型
			using element_type = std::remove_const_t<sound_handler::element_type>;
			// ResourceManager返回给用户的类型
			using result_type = sound_handler;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> entt::resource<element_type>;
			[[nodiscard]] auto operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>;
		};

		class MusicLoader final
		{
		public:
			// ResourceManager内部entt::resource_cache保存的类型
			using element_type = std::remove_const_t<music_handler::element_type>;
			// ResourceManager返回给用户的类型
			using result_type = music_handler;

			[[nodiscard]] auto operator()(std::string_view path) const noexcept -> entt::resource<element_type>;
			[[nodiscard]] auto operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>;
		};
	}

	class Font final : public resource_detail::ResourceManager<resource_detail::FontLoader> {};

	class Texture final : public resource_detail::ResourceManager<resource_detail::TextureLoader> {};

	class Sound final : public resource_detail::ResourceManager<resource_detail::SoundLoader> {};

	class Music final : public resource_detail::ResourceManager<resource_detail::MusicLoader> {};
}
