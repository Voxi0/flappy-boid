// Raylib
#define RAYGUI_IMPLEMENTATION
#include<raylib.h>
#include<raygui.h>

// Emscripten
#if defined(PLATFORM_WEB)
	#include "../.emscripten_cache/sysroot/include/emscripten.h"
#endif

// Custom headers
#include<bird.h>
#include<pipe.h>

// Function prototypes
void update(void);
static void drawMainMenu(void);

// Main window
#define WIN_TITLE "flappy-boid"
unsigned int winWidth = 0, winHeight = 0;

// Game state
typedef enum {
	STATE_MENU,
	STATE_PLAYING,
	STATE_GAMEOVER,
	STATE_EXIT,
} GameState_t;
GameState_t gameState = STATE_MENU;

// Main camera
Camera2D cam = {
	.target = (Vector2){0.0f, 0.0f},
	.offset = (Vector2){0.0f, 0.0f},
	.rotation = 0.0f,
	.zoom = 1.0f,
};

// Bird/Player
Bird bird = {
	.size = 0.1f,
	.gravity = 600,
	.jumpVel = 320,
};

// Pipe
Texture2D pipeTexture;
Pipe pipes[2] = {0};

// Ground
Rectangle box = {0};

// Main
int main(void) {
	// Main window
	InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), WIN_TITLE);
	winWidth = GetScreenWidth();
	winHeight = GetScreenHeight();

	// Ground
	box.width = winWidth;
	box.height = 100;
	box.x = 0;
	box.y = winHeight - box.height;

	// Load bird/player texture and create a source and destination rectangle for it
	bird.sprite = LoadTexture("assets/sprites/bird.png");
	bird.src = (Rectangle) {
		.x = 0,
		.y = 0,
		.width = bird.sprite.width,
		.height = bird.sprite.height,
	};
	bird.dst = (Rectangle) {
		.x = 0,
		.y = 0,
		.width = bird.sprite.width * bird.size,
		.height = bird.sprite.height * bird.size,
	};

	// Pipes
	pipeTexture = LoadTexture("assets/sprites/pipe.png");

	// Top pipe
	pipes[0].moveSpeed = 90.0f;
	pipes[0].src = (Rectangle) {
		.x = 0,
		.y = 0,
		.width = pipeTexture.width,
		.height = pipeTexture.height,
	};
	pipes[0].dst = (Rectangle) {
		.x = winWidth,
		.y = 0,
		.width = 150,
		.height = 200,
	};

	// Bottom pipe
	pipes[1].moveSpeed = 90.0f;
	pipes[1].src = (Rectangle) {
		.x = 0,
		.y = 0,
		.width = pipeTexture.width,
		.height = -pipeTexture.height,
	};
	pipes[1].dst = (Rectangle) {
		.x = winWidth,
		.width = 150,
		.height = 200,
	};
	pipes[1].dst.y = box.y - pipes[1].dst.height,

	// Main loop
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update, 0, 1);
#else
	SetTargetFPS(60);

	// Main loop
	while (!WindowShouldClose() && gameState != STATE_EXIT) update();
#endif

	// Terminate program
	UnloadTexture(bird.sprite);
	UnloadTexture(pipeTexture);
	CloseWindow();
	return 0;
}

// Main loop
void update(void) {
	// Update everything
	switch(gameState) {
		case STATE_PLAYING:
			// Update player and pipes
			birdUpdate(&bird);
			pipeUpdate(&pipes[0]);
			pipeUpdate(&pipes[1]);

			// Check if player hit the ground
			if(CheckCollisionRecs(bird.dst, box)) gameState = STATE_GAMEOVER;
			
			// Check if player hit any of the pipes
			for(int i = 0; i < sizeof(pipes) / sizeof(pipes[0]); i++) {
				if(CheckCollisionRecs(bird.dst, pipes[i].dst)) gameState = STATE_GAMEOVER;
			}
			break;
		default:
			break;
	};

	// Render everything
	BeginDrawing();
		switch(gameState) {
			// Main menu
			case STATE_MENU:
				ClearBackground(GRAY);
				drawMainMenu();
				break;

			// Game over
			case STATE_GAMEOVER:
				ClearBackground(RED);
				Vector2 size = MeasureTextEx(GetFontDefault(), "GAME OVER.", 40, 1);
				DrawText("GAME OVER.", (GetScreenWidth() - size.x) / 2, (GetScreenHeight() - size.y) / 2, 40, WHITE);
				break;

			// Draw the game and all
			case STATE_PLAYING:
				// Clear screen
				ClearBackground(RAYWHITE);
				DrawFPS(10, 10);

				// Render everything
				BeginMode2D(cam);
					DrawRectangleRec(box, RED);
					DrawTexturePro(bird.sprite, bird.src, bird.dst, (Vector2) {0, 0}, 0, WHITE);
					DrawTexturePro(pipeTexture, pipes[0].src, pipes[0].dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
					DrawTexturePro(pipeTexture, pipes[1].src, pipes[1].dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
				EndMode2D();
				break;

			default:
				break;
		}
	EndDrawing();
}

// Main menu
static void drawMainMenu(void) {
	// Define buttons
	const char *buttonLabels[] = { "Play", "Quit" };
	const int numButtons = sizeof(buttonLabels) / sizeof(char*);

	// Button size and stuff
	const int btnWidth = 500, btnHeight = 50;
	const int btnGap = 5;

	// Calculate button position
	const int btnStartX = (GetScreenWidth() - btnWidth) / 2;
	const int btnStartY = (GetScreenHeight() - (btnHeight * numButtons)) / 2;

	// Title
	Vector2 size = MeasureTextEx(GetFontDefault(), "Flappy-Boid", 40, 1);
	DrawTextPro(GetFontDefault(), "Flappy-Boid", (Vector2){GetScreenWidth() / 2.0f, (btnStartY - size.y) - btnGap}, (Vector2){size.x / 2, size.y / 2}, 0.0f, 40.0f, 1.0f, RED);

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