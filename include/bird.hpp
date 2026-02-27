#pragma once

#include <string_view>
#include <raylib.h>
#include <raymath.h>

struct Bird {
	public:
		Rectangle srcRect {}, dstRect {};
		float jumpForce {-350.0f};

		void init(std::string_view textureFilePath, float xpos, float scale);
		void update(int gravity);
		void render(void);
		~Bird(void);

	private:
		Texture2D texture {};
		Vector2 velocity {};
		float rotation {};
};