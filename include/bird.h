#include<raylib.h>

typedef struct {
	Texture2D sprite;
	Vector2 position, velocity;
	Rectangle src, rec;
	int gravity, jumpVel;
	const float size;
} Bird;

void birdUpdate(Bird *bird);