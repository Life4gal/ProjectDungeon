// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/actor.hpp>

#include <component/actor.hpp>

#include <entt/entity/registry.hpp>

namespace pd::factory::detail
{
	using namespace component;

	namespace
	{
		class Frame final
		{
		public:
			int width;
			int height;
			int origin_x;
			int origin_y;
		};

		auto do_attach(entt::registry& registry, const entt::entity entity, const blueprint::Actor& actor, const Frame& frame) noexcept -> void
		{
			// 生命值条|魔法值条与精灵的最小偏移
			const auto base_offset_x = static_cast<float>(frame.origin_x);
			const auto base_offset_y = static_cast<float>(frame.origin_y) + 5.f;

			// 生命值条|魔法值条的宽度&高度
			const auto bar_width = static_cast<float>(frame.width) * 1.2f;
			const auto bar_height = static_cast<float>(frame.height) / 4;

			// 偏移1(上面那根)
			const auto offset_x_1 = -(base_offset_x + (bar_width - static_cast<float>(frame.width)) / 2);
			const auto offset_y_1 = -(base_offset_y + bar_height * 2);
			// 偏移2(下面那根)
			const auto offset_x_2 = offset_x_1;
			const auto offset_y_2 = -(base_offset_y + bar_height * 1);

			// 生命值
			registry.emplace<actor::Health>(entity, actor.health);
			registry.emplace<actor::HealthMax>(entity, actor.health);
			// 魔法值
			registry.emplace<actor::Mana>(entity, actor.mana);
			registry.emplace<actor::ManaMax>(entity, actor.mana);
			// 速度
			registry.emplace<actor::Speed>(entity, actor.speed);
			// 生命值条&魔法值条
			if (actor.mana > 0)
			{
				// 如果有魔法值时有两条,生命值条在上,魔法值条在下
				registry.emplace<actor::HealthBarSize>(entity, sf::Vector2f{bar_width, bar_height});
				registry.emplace<actor::HealthBarOffset>(entity, sf::Vector2f{offset_x_1, offset_y_1});
				registry.emplace<actor::ManaBarSize>(entity, sf::Vector2f{bar_width, bar_height});
				registry.emplace<actor::ManaBarOffset>(entity, sf::Vector2f{offset_x_2, offset_y_2});
			}
			else
			{
				// 没有魔法值时只有一条生命值条
				registry.emplace<actor::HealthBarSize>(entity, sf::Vector2f{bar_width, bar_height});
				registry.emplace<actor::HealthBarOffset>(entity, sf::Vector2f{offset_x_2, offset_y_2});

				// render::health_mana_bar中依然会读取这两个组件,不过没有魔法值时它们的值无所谓
				registry.emplace<actor::ManaBarSize>(entity, sf::Vector2f{bar_width, bar_height});
				registry.emplace<actor::ManaBarOffset>(entity, sf::Vector2f{0, 0});
			}
		}
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Actor& actor, const blueprint::Sprite& sprite) noexcept -> void
	{
		const Frame frame{.width = sprite.width, .height = sprite.height, .origin_x = sprite.origin_x, .origin_y = sprite.origin_y};

		do_attach(registry, entity, actor, frame);
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Actor& actor, const blueprint::SpriteAnimation& sprite_animation) noexcept -> void
	{
		const Frame frame{.width = sprite_animation.width, .height = sprite_animation.height, .origin_x = sprite_animation.origin_x, .origin_y = sprite_animation.origin_y};

		do_attach(registry, entity, actor, frame);
	}
}
