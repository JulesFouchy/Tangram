#include "cursor.hpp"

#include "stb_image/stb_image.h"

#include "UI/log.hpp"

CursorType* Cursor::standard;
CursorType* Cursor::hand;
CursorType* Cursor::leftRight;
CursorType* Cursor::antiDiagonal;
CursorType* Cursor::topBot;
CursorType* Cursor::diagonal;
CursorType* Cursor::fourDirections;
CursorType* Cursor::wait;
CursorType* Cursor::rotate;

void Cursor::Initialize() {
	standard =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	hand =				SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	leftRight =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	antiDiagonal =		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	topBot =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	diagonal =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	fourDirections =	SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	wait =				SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
	rotate =			createCustomCursor("res/cursors/curved_arrow.png");
}

void Cursor::ShutDown() {
	SDL_FreeCursor(standard);
	SDL_FreeCursor(hand);
	SDL_FreeCursor(leftRight);
	SDL_FreeCursor(antiDiagonal);
	SDL_FreeCursor(topBot);
	SDL_FreeCursor(diagonal);
	SDL_FreeCursor(fourDirections);
	SDL_FreeCursor(wait);
	SDL_FreeCursor(rotate);
}

void Cursor::set(CursorType* cursorPtr) {
	SDL_SetCursor(cursorPtr);
}

CursorType* Cursor::createCustomCursor(std::string imagePath) {
	int req_format = STBI_rgb_alpha;
	int width, height, orig_format;
	unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &orig_format, req_format);
	if (data == nullptr) {
		spdlog::warn("[Cursor] unable to open file {}", imagePath);
	}

	// Set up the pixel format color masks for RGB(A) byte arrays.
	// Only STBI_rgb (3) and STBI_rgb_alpha (4) are supported here!
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int shift = (req_format == STBI_rgb) ? 8 : 0;
	rmask = 0xff000000 >> shift;
	gmask = 0x00ff0000 >> shift;
	bmask = 0x0000ff00 >> shift;
	amask = 0x000000ff >> shift;
#else // little endian, like x86
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = (req_format == STBI_rgb) ? 0 : 0xff000000;
#endif

	int depth, pitch;
	if (req_format == STBI_rgb) {
		depth = 24;
		pitch = 3 * width; // 3 bytes per pixel * pixels per row
	}
	else { // STBI_rgb_alpha (RGBA)
		depth = 32;
		pitch = 4 * width;
	}

	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch, rmask, gmask, bmask, amask);
	if (surf == nullptr) {
		spdlog::warn("[Cursor] unable to create sdl surface : {}", SDL_GetError());
	}
	return SDL_CreateColorCursor(surf, width / 2, height / 2);
}