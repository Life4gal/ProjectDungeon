#include <game/game.hpp>
#include <game/constants.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

auto main() noexcept -> int
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	using namespace pd;

	Game game{Constant::window_width, Constant::window_height, Constant::window_title, false};
	game.run();
}
