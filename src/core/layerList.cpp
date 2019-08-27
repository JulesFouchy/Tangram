#include "layerList.hpp"

#include "drawingBoard.hpp"

#include "UI/log.hpp"

#include "UI/input.hpp"

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
	/*
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->showFrame();
	}
	*/
	getActivLayer()->showFrame();

	if (Input::keyIsDown(ALT)) {
		getActivLayer()->m_transform.showAltOrigin();
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

void LayerList::onDoubleLeftClic() {
	if (Input::keyIsDown(ALT) && glm::length(m_activLayer->m_transform.getAltOriginInWindowSpace() - Input::getMousePosition()) < 0.05f) {
		m_activLayer->m_transform.setAltOrigin(glm::vec2(0.0f));
	}
}

void LayerList::onLeftClicDown() {
	if (Input::keyIsDown(ALT) && glm::length(m_activLayer->m_transform.getAltOriginInWindowSpace() - Input::getMousePosition()) < 0.05f) {
		m_activLayer->m_transform.startDraggingAltOrigin();
	}
	else if (m_hoveredLayer)
	{
		//Change activ layer
		m_activLayer = m_hoveredLayer;
		m_bIsHandlingAnInput = true;
		//Drag translation if inside
		if (m_mousePosRelToHoveredLayer == INSIDE) {
			m_hoveredLayer->m_transform.startDraggingTranslation();
		}
		//Scale towards opposite border
		else {
			switch (m_mousePosRelToHoveredLayer)
			{
			case OUTSIDE:
				spdlog::error("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're outside any layer");
				break;
			case INSIDE:
				spdlog::error("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're inside any layer");
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
				spdlog::error("[LayerList::onLeftClicDown] reached default case");
				break;
			}
			//Scale towards center if ALT down
			if (Input::keyIsDown(ALT)) {
				m_hoveredLayer->m_transform.changeToAltDraggingScaleOrigin();
			}
		}
	}
}

void LayerList::onLeftClicUp() {
	m_activLayer->m_transform.endDragging();
	m_bIsHandlingAnInput = false;
	usedCursor = nullptr;
}

void LayerList::setCursor() {
	if (isHandlingAnInput() && usedCursor) {
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
			spdlog::error("[LayerList::SetCursor] reached default case");
			break;
		}
	}
}