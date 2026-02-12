// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/asset.hpp>

#include <ctx/asset_loader.hpp>

#include <entt/entt.hpp>

namespace pd::systems::initialize
{
	auto asset(entt::registry& registry) noexcept -> void
	{
		// 创建
		// todo: 需不需要提前创建?
		ctx::FontLoader::emplace(registry);
		ctx::TextureLoader::emplace(registry);
		ctx::SoundLoader::emplace(registry);
		ctx::MusicLoader::emplace(registry);

		// 加载
		ctx::FontLoader::load(registry);
		ctx::TextureLoader::load(registry);
		ctx::SoundLoader::load(registry);
		ctx::MusicLoader::load(registry);
	}
}
