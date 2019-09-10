#include "history.hpp"

#include "UI/log.hpp"

History::History()
	: m_currentDelimiterIndex(-1)
{
	/*beginUndoGroup();
		addAction(Action([] {
			spdlog::info("do initial");
		}, [] {
			spdlog::info("undo initial");
		}));
	endUndoGroup();*/
}

History::~History() {

}

void History::beginUndoGroup() {
	if (m_actions.size() == 0) {
		m_undoGroupDelimiters.push_back(0);
	}
	Log::separationLine();
	spdlog::warn("begin   size : {}", m_actions.size());
	if (m_currentDelimiterIndex < m_undoGroupDelimiters.size() - 1) {
		m_actions.resize(getCurrentDelimiter() + 1);
		m_undoGroupDelimiters.resize(m_currentDelimiterIndex + 1);
	}
	spdlog::warn("begin resize : {}", m_actions.size());
}

void History::endUndoGroup() {
	if (m_actions.size() - 1 != m_undoGroupDelimiters[m_undoGroupDelimiters.size() - 1]) {
		m_undoGroupDelimiters.push_back(m_actions.size() - 1); //Put a delimiter at the end of the actions list
		m_currentDelimiterIndex = m_undoGroupDelimiters.size() - 1; //This delimiter becomes the current one
		spdlog::warn("end     size : {}", m_actions.size());
	}
	else
		spdlog::warn("Empty undo group");
}

void History::addAction(Action action) {
	m_actions.push_back(action);
}

/*
 - We should always be in the state m_actions[getCurrentDelimiter()]
*/

void History::moveBackward() {
	if (m_currentDelimiterIndex > 0) {
		int prevDelimiter = m_undoGroupDelimiters[m_currentDelimiterIndex - 1];
		int currDelimiter = m_undoGroupDelimiters[m_currentDelimiterIndex];
		spdlog::info("back from {} to {}", currDelimiter, prevDelimiter);
		for (int k = currDelimiter; k >= prevDelimiter + 1; --k) {
			spdlog::info("undoing action {}", k);
			m_actions[k].Undo();
		}
		m_currentDelimiterIndex--;
	}
}

void History::moveForward() {
	if (m_currentDelimiterIndex < m_undoGroupDelimiters.size()-1) {
		int nextDelimiter = m_undoGroupDelimiters[m_currentDelimiterIndex + 1];
		int currDelimiter = m_undoGroupDelimiters[m_currentDelimiterIndex];
		spdlog::info("forward from {} to {}", currDelimiter, nextDelimiter);
		for (int k = currDelimiter + 1; k <= nextDelimiter; ++k) {
			spdlog::info("doing action {}", k);
			m_actions[k].Do();
		}
		m_currentDelimiterIndex++;
	}
}

int History::getCurrentDelimiter() {
	return m_undoGroupDelimiters[m_currentDelimiterIndex];
}