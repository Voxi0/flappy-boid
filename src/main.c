#define RAYGUI_IMPLEMENTATION
#include<raylib.h>
#include<raygui.h>

#include<bird.h>

#if defined(PLATFORM_WEB)
	#include "../.emscripten_cache/sysroot/include/emscripten.h"
#endif

#define WIN_TITLE "flappy-boid"

// Game state
typedef enum {
	STATE_MENU,
	STATE_PLAYING,
	STATE_GAMEOVER,
	STATE_EXIT,
} GameState_t;
GameState_t gameState = STATE_MENU;

// Function prototypes
void update(void);
static void drawMainMenu(void);

// Main camera
Camera2D cam = {0};

// Bird/Player
Bird bird = {
	.size = 0.1f,
};
Rectangle box = {0};

// Main
int main(void) {
	// Main window
	InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), WIN_TITLE);

	cam.target = (Vector2){0.0f, 0.0f};
	cam.offset = (Vector2){0.0f, 0.0f};
	cam.rotation = 0.0f;
	cam.zoom = 1.0f;

	bird.sprite = LoadTexture("assets/sprites/bird.png");
	bird.gravity = 600;
	bird.jumpVel = 320;
	bird.src = (Rectangle) {
		0, 0,
		bird.sprite.width, bird.sprite.height
	};
	bird.dst = (Rectangle) {
		0, 0,
		bird.sprite.width * bird.size,
		bird.sprite.height * bird.size
	};

	box.x = GetScreenWidth() / 2.0f;
	box.y = GetScreenHeight() - 200;
	box.width = GetScreenWidth();
	box.height = 100;

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update, 0, 1);
#else
	SetTargetFPS(60);

	// Main loop
	while (!WindowShouldClose() && gameState != STATE_EXIT) update();
#endif

	// Terminate program
	UnloadTexture(bird.sprite);
	CloseWindow();
	return 0;
}

// Main loop
void update(void) {
	switch(gameState) {
		// Main menu
		case STATE_MENU:
			BeginDrawing();
				ClearBackground(GRAY);
				drawMainMenu();
			EndDrawing();
			break;

		// Game over
		case STATE_GAMEOVER:
			BeginDrawing();
				ClearBackground(RED);
				Vector2 size = MeasureTextEx(GetFontDefault(), "GAME OVER.", 40, 1);
				DrawText("GAME OVER.", (GetScreenWidth() - size.x) / 2, (GetScreenHeight() - size.y) / 2, 40, WHITE);
			EndDrawing();
			break;

		// Draw the game and all
		case STATE_PLAYING:
			birdUpdate(&bird);
			BeginDrawing();
				ClearBackground(RAYWHITE);
				DrawFPS(10, 10);
				if(CheckCollisionRecs(bird.dst, box)) gameState = STATE_GAMEOVER;
				BeginMode2D(cam);
					// Draw bird/player and box
					DrawRectangleRec(box, RED);
					DrawTexturePro(bird.sprite, bird.src, bird.dst, (Vector2) {0, 0}, 0, WHITE);
				EndMode2D();
			EndDrawing();
			break;

		default:
			break;
	}
}

// Main menu
static void drawMainMenu(void) {
	// Define buttons
	const char *buttonLabels[] = { "Play", "Quit" };
	const int numButtons = sizeof(buttonLabels) / sizeof(char*);

	// Button size and stuff
	const int btnWidth = 300, btnHeight = 30;
	const int btnGap = 5;

	// Calculate button position
	const int btnStartX = (GetScreenWidth() - btnWidth) / 2;
	const int btnStartY = (GetScreenHeight() - (btnHeight * numButtons)) / 2;

	// Create buttons
	for (int i = 0; i < numButtons; i++) {
        Rectangle btnRec = { btnStartX, btnStartY + i * (btnHeight + btnGap), btnWidth, btnHeight };
        if (GuiButton(btnRec, buttonLabels[i])) {
            switch (i) {
                case 0:
					gameState = STATE_PLAYING;
					break;
                case 1:
					gameState = STATE_EXIT;
					break;
            }
        }
    }
}