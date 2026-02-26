#pragma once

#include <raylib.h>

struct Pipe {
	public:
		Rectangle srcRect {}, dstRect {};
		int moveSpeed {90}, pipeGap {220};

		void spawn(float x, float gapY, float gapHeight, bool isTopPipe);
		void update(void);
		void render(Texture2D &texture);
	
	private:
		bool passed {false};
};