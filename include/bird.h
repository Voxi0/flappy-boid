#include<raylib.h>

typedef struct {
	Texture2D sprite;
	Vector2 velocity;
	Rectangle src, dst;
	int gravity, jumpVel;
	const float size;
} Bird;

void birdUpdate(Bird *bird);