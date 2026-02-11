#include <game/game.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

auto main() noexcept -> int
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	pd::Game game{1920, 1080, "ProjectDungeon", false};
	game.run();
}
