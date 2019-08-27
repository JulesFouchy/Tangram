#include "cursor.hpp"

CursorStyle* Cursor::standard;
CursorStyle* Cursor::hand;
CursorStyle* Cursor::leftRight;
CursorStyle* Cursor::antiDiagonal;
CursorStyle* Cursor::topBot;
CursorStyle* Cursor::diagonal;
CursorStyle* Cursor::fourDirections;

void Cursor::Initialize() {
	standard =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	hand =				SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	leftRight =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	antiDiagonal =		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	topBot =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	diagonal =			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	fourDirections =	SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
}

void Cursor::ShutDown() {
	SDL_FreeCursor(standard);
	SDL_FreeCursor(hand);
	SDL_FreeCursor(leftRight);
	SDL_FreeCursor(antiDiagonal);
	SDL_FreeCursor(topBot);
	SDL_FreeCursor(diagonal);
	SDL_FreeCursor(fourDirections);
}

void Cursor::set(CursorStyle* cursorPtr) {
	SDL_SetCursor(cursorPtr);
}