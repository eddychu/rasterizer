#pragma once
#include <algorithm>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
struct PixelBuffer
{
	PixelBuffer(int w, int h)
		: width(w)
		, height(h)
	{
		pixels = new Uint32[width * height]();
	}
	~PixelBuffer() {
		delete[] pixels;
	}

	Uint32* GetPixels() {
		return pixels;
	}

	void SetColor(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		int index = y * width + x;
		pixels[index] = SDL_MapRGB(format, r, g, b);
	}
	void Clear() {
		memset(pixels, 0xD, width * sizeof(Uint32) * height);
	}

	int GetWidth() {
		return width;
	}

	int GetHeight() {
		return height;
	}

	int width;
	int height;
	Uint32* pixels;
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
};