// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Time.hpp>
// #include <SFML/Graphics/Font.hpp>
// #include <SFML/Graphics/Text.hpp>
// #include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Music.hpp>

namespace pd::components
{
	// 此帧历时
	class FrameDelta final
	{
	public:
		sf::Time delta;
	};

	// 总历时
	class SceneElapsedTime final
	{
	public:
		sf::Time elapsed;
	};

	// 实际总历时(不算暂停时间)
	class SceneRealElapsedTime final
	{
	public:
		sf::Time elapsed;
	};

	// // 游戏字体
	// class Font final
	// {
	// public:
	// 	sf::Font font;
	// };
	//
	// // 游戏文本
	// class Text final
	// {
	// public:
	// 	sf::Text text;
	// };

	// 游戏音乐
	class BackgroundMusic final
	{
	public:
		sf::Music music;
	};
}
