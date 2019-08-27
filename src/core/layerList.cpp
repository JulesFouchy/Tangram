#include "layerList.hpp"

#include "drawingBoard.hpp"

#include "UI/log.hpp"

LayerList::LayerList()
	: m_activLayer(nullptr), m_hoveredLayer(nullptr), m_mousePosRelToHoveredLayer(OUTSIDE), m_bIsHandlingAnInput(false), usedCursor(nullptr)
{
}

LayerList::~LayerList() {

}

void LayerList::update() {
	computeHoveredLayerAndMouseRelPos();
	if(m_activLayer)
		m_activLayer->m_transform.checkDragging();
}

void LayerList::show() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->show();
	}
}

void LayerList::showFrames() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->showFrame();
	}
}

void LayerList::addLayer(std::string imgFilePath) {
	layers.push_back(new Layer(imgFilePath));
	setActivLayer(layers.size() - 1);
}

Layer* LayerList::getActivLayer() {
	return m_activLayer;
}
void LayerList::setActivLayer(int layerIndex) {
	m_activLayer = layers[layerIndex];
}

void LayerList::computeHoveredLayerAndMouseRelPos() {
	m_hoveredLayer = nullptr;
	for (int k = layers.size()-1; k >= 0; --k) {
		m_mousePosRelToHoveredLayer = layers[k]->m_transform.getMouseRelativePosition();
		if (m_mousePosRelToHoveredLayer != OUTSIDE) {
			m_hoveredLayer = layers[k];
			break;
		}
	}
}

void LayerList::onLeftClicDown() {
	if (m_hoveredLayer)
	{
		m_activLayer = m_hoveredLayer;
		m_bIsHandlingAnInput = true;
		switch (m_mousePosRelToHoveredLayer)
		{
		case OUTSIDE:
			spdlog::warn("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're outside any layer");
			break;
		case INSIDE:
			m_hoveredLayer->m_transform.startDraggingTranslation();
			break;
		case RIGHT:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.0f));
			break;
		case TOP_RIGHT:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), -0.5f));
			break;
		case TOP:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(0.0f, -0.5f));
			break;
		case TOP_LEFT:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), -0.5f));
			break;
		case LEFT:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.0f));
			break;
		case BOT_LEFT:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.5f));
			break;
		case BOT:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(0.0f, 0.5f));
			break;
		case BOT_RIGHT:
			m_hoveredLayer->m_transform.startDraggingScale(glm::vec2(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.5f));
			break;
		default:
			break;
		}
	}
}

void LayerList::onLeftClicUp() {
	m_activLayer->m_transform.endDragging();
	m_bIsHandlingAnInput = false;
	usedCursor = nullptr;
}

void LayerList::setCursor() {
	if (m_bIsHandlingAnInput && usedCursor) {
		Cursor::set(usedCursor);
	}
	else {
		switch (m_mousePosRelToHoveredLayer)
		{
		case OUTSIDE:
			usedCursor = nullptr;
			Cursor::set(Cursor::standard);
			break;
		case INSIDE:
			usedCursor = Cursor::hand;
			Cursor::set(Cursor::hand);
			break;
		case RIGHT:
			usedCursor = Cursor::leftRight;
			Cursor::set(Cursor::leftRight);
			break;
		case TOP_RIGHT:
			usedCursor = Cursor::antiDiagonal;
			Cursor::set(Cursor::antiDiagonal);
			break;
		case TOP:
			usedCursor = Cursor::topBot;
			Cursor::set(Cursor::topBot);
			break;
		case TOP_LEFT:
			usedCursor = Cursor::diagonal;
			Cursor::set(Cursor::diagonal);
			break;
		case LEFT:
			usedCursor = Cursor::leftRight;
			Cursor::set(Cursor::leftRight);
			break;
		case BOT_LEFT:
			usedCursor = Cursor::antiDiagonal;
			Cursor::set(Cursor::antiDiagonal);
			break;
		case BOT:
			usedCursor = Cursor::topBot;
			Cursor::set(Cursor::topBot);
			break;
		case BOT_RIGHT:
			usedCursor = Cursor::diagonal;
			Cursor::set(Cursor::diagonal);
			break;
		default:
			spdlog::warn("[LayerList::SetCursor] reached default case");
			break;
		}
	}
}