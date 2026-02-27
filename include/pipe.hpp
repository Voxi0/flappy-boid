#pragma once

#include <raylib.h>

struct Pipe {
	public:
		Rectangle srcRect {}, dstRect {.width = 50};
		int moveSpeed {90};
		bool active {false};

		void spawn(float x, float gap, float gapTopY, bool isTopPipe);
		void update(void);
		void render(Texture2D &base, Texture2D &cap, Rectangle &baseSrc, Rectangle &capSrc);
	
	private:
		bool passed {false};
};