#include "history.hpp"

#include "UI/log.hpp"

History::History()
	: m_currentIndex(-1)
{

}

History::~History() {

}

void History::addUndoGroup(UndoGroup undo) {
	spdlog::info("add");
	m_undoGroups.push_back(undo);
	m_currentIndex++;
}

void History::moveBack() {
	spdlog::info("backward");
	spdlog::info(m_currentIndex);
	if (m_currentIndex > 0) {
		m_currentIndex--;
		m_undoGroups[m_currentIndex]();
	}
}

void History::moveForward() {
	spdlog::info("forward");
	spdlog::info(m_currentIndex);
	if (m_currentIndex < m_undoGroups.size()-1) {
		m_currentIndex++;
		m_undoGroups[m_currentIndex]();
	}
}