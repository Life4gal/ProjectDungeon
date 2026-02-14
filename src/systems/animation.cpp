// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/animation.hpp>

#include <systems/world.hpp>
#include <systems/asset.hpp>

#include <components/transform.hpp>
#include <components/animation.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace pd::systems
{
	auto Animation::do_loaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
		// 相关实体由Blueprint::initialize创建
		// registry.ctx().emplace<blueprint::AnimationSet>();
	}

	auto Animation::do_initialized([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Animation::do_unloaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Animation::do_update(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;

		// 如果暂停游戏则无需更新动画
		if (World::is_pause(registry))
		{
			return;
		}

		for (const auto view = registry.view<AnimationTimer, AnimationIndex>(
			     entt::exclude<
				     // 如果动画暂停则无需更新动画
				     AnimationPaused,
				     // 如果动画已经结束(例如非循环动画播完了最后一帧)则无需更新动画
				     AnimationEnded
			     >
		     );
		     const auto [entity, timer, index]: view.each())
		{
			timer.elapsed += delta;
			if (timer.elapsed < timer.duration)
			{
				continue;
			}

			timer.elapsed -= timer.duration;
			index.current += 1;

			if (index.current < index.total)
			{
				continue;
			}

			if (registry.all_of<AnimationLooping>(entity))
			{
				// 循环动画
				index.current %= index.total;
			}
			else
			{
				// 对于非循环动画,如果已经超过最后一帧则标记为已结束
				registry.emplace<AnimationEnded>(entity);
			}
		}
	}

	auto Animation::do_render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		using namespace components;

		// 当前共用RenderStates
		static sf::RenderStates states{};

		for (const auto view = registry.view<
			     const Position,
			     const Scale,
			     const Rotation,
			     const AnimationIndex,
			     const AnimationView>();
		     const auto [entity, position, scale, rotation, animation_index, animation_view]: view.each())
		{
			const auto& frame = animation_view.animation->frames[animation_index.current];
			const auto texture = Asset::texture(registry, frame.texture);

			if (not texture)
			{
				// todo: 默认纹理?
				SPDLOG_ERROR(
					"无法渲染位于({:.3f}:{:.3f})的实体[{}],纹理ID: {}",
					position.position.x,
					position.position.y,
					entt::to_integral(entity),
					frame.texture
				);
				continue;
			}

			const auto x = static_cast<float>(frame.frame_x);
			const auto y = static_cast<float>(frame.frame_y);
			const auto width = static_cast<float>(frame.frame_width);
			const auto height = static_cast<float>(frame.frame_height);
			// FIXME: 总是假定原点为正中心(width/2, height/2)
			const auto origin = sf::Vector2f{width, height} / 2.f;

			const auto vertices = [&] noexcept -> auto
			{
				// todo: 能不能控制颜色?
				// 可以做一些闪烁效果
				constexpr auto color = sf::Color::White;

				const auto right = x + width;
				const auto bottom = y + height;

				return std::array<sf::Vertex, 4>
				{{
						{.position = {0, 0}, .color = color, .texCoords = {x, y}},
						{.position = {width, 0}, .color = color, .texCoords = {right, y}},
						{.position = {0, height}, .color = color, .texCoords = {x, bottom}},
						{.position = {width, height}, .color = color, .texCoords = {right, bottom}},
				}};
			}();

			states.texture = texture.operator->();
			states.transform = [&] noexcept -> sf::Transform
			{
				sf::Transform t{};
				t.translate(position.position);
				t.scale(scale.scale);
				t.rotate(rotation.rotation);
				t.translate(-origin);

				return t;
			}();

			window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip, states);
		}
	}

	auto Animation::get(entt::registry& registry, const std::string_view name) noexcept -> blueprint::AnimationView
	{
		auto& animations = registry.ctx().get<blueprint::AnimationSet>();

		if (const auto it = animations.find(name);
			it != animations.end())
		{
			return it->second;
		}

		// todo: 备用动画?
		return animations.begin()->second;
	}

	auto Animation::pause(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.emplace_or_replace<components::AnimationPaused>(entity);
	}

	auto Animation::unpause(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.remove<components::AnimationPaused>(entity);
	}

	auto Animation::looping(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.emplace_or_replace<components::AnimationLooping>(entity);

		// 如果非循环动画之前已经播放完毕,则需要移除已结束的标记
		registry.remove<components::AnimationEnded>(entity);
	}

	auto Animation::unlooping(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.remove<components::AnimationLooping>(entity);
	}
}
