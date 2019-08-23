#include "timestamp.hpp"

#include "SDL.h"

Timestamp::Timestamp() {
	m_creationDate = getTime();
}
Timestamp::~Timestamp() {

}

float Timestamp::timeSince(Timestamp thatDate) {
	return m_creationDate - thatDate.m_creationDate;
}

float Timestamp::getAge() {
	return getTime() - m_creationDate;
}

float Timestamp::getTime() {
	return (float)SDL_GetTicks() / 1000.0f;
}

void Timestamp::reset() {
	m_creationDate = getTime();
}