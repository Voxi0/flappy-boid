#include <game.hpp>
#define WIN_TITLE "test"

// Main
int main(void) {
	Game game(0, 0, WIN_TITLE, FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	game.run();
	return 0;
}