#pragma once

#include <string>
#include <raylib.h>
#include <raymath.h>

struct Bird {
	public:
		Texture2D texture {};
		Rectangle srcRect {}, dstRect {};
		float gravity {980.0f}, jumpForce {-350.0f};

		void init(std::string_view textureFilePath);
		void update(void);
		void render(void);
		~Bird(void);

	private:
		Vector2 velocity {};
		float rotation {};
};