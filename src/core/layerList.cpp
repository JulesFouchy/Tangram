#include "layerList.hpp"

#include "drawingBoard.hpp"

#include "UI/log.hpp"

#include "UI/input.hpp"

#include "UI/settings.hpp"

#include "helper/display.hpp"
#include "graphics/immediateDrawing.hpp"

LayerList::LayerList()
	: m_hoveredLayer(0), m_mousePosRelToHoveredLayer(OUTSIDE), usedCursor(nullptr)
{
}

LayerList::~LayerList() {

}

void LayerList::update() {
	computeHoveredLayerAndMouseRelPos();
	selectedLayers.checkDragging();
}

void LayerList::show(glm::mat4x4 viewMatrix, glm::mat4x4 projMatrix) {
	for (int k = 0; k < layers.size(); ++k) {
		if (DrawingBoard::LayerRegistry()[layers[k]]->isVisible())
			DrawingBoard::LayerRegistry()[layers[k]]->show(viewMatrix, projMatrix);
	}
}

void LayerList::showForSaving() {
	for (int k = 0; k < layers.size(); ++k) {
		if (DrawingBoard::LayerRegistry()[layers[k]]->isVisible())
			DrawingBoard::LayerRegistry()[layers[k]]->showForSaving();
	}
}

void LayerList::showSelectedLayerGUI() {
	if (selectedLayers.size() == 1)
		DrawingBoard::LayerRegistry()[selectedLayers[0]]->showGUI();
}

/*void LayerList::showFrames() {
	//for (int k = 0; k < layers.size(); ++k) {
	//	layers[k]->showFrame();
	//}
	
	if (getActivLayer()->isVisible()) {
		getActivLayer()->showFrame();

		if (mustShowAltOrigin()) {
			getActivLayer()->m_transform.showAltOrigin();
		}
	}

}*/

/*Layer* LayerList::getActivLayer() {
	return selectedLayers.;
}*/
void LayerList::setSelectedLayer(LayerID layerID) {
	selectedLayers.removeAllLayers();
	selectedLayers.addLayer(layerID);
}

void LayerList::computeHoveredLayerAndMouseRelPos() {
	m_hoveredLayer = 0;
	for (int k = layers.size()-1; k >= 0; --k) {
		Layer* layer = DrawingBoard::LayerRegistry()[layers[k]];
		if (layer->isVisible()) {
			// get mouse position
			glm::vec2 mousePosInNTS = layer->m_transform.getMousePositionInNormalizedTransformSpace();
			m_mousePosRelToHoveredLayer = layer->m_transform.getRelativePositionFromPositionInNormalizedTransformSpace(mousePosInNTS);
			// check if hovered part of the layer is not fully transparent
			if (m_mousePosRelToHoveredLayer == INSIDE) {
				mousePosInNTS += glm::vec2(0.5f); // 0to1, 0to1
				// get pixel color under mouse
				unsigned char pixelColor[4];
				FrameBuffer& layerFrameBuffer = layer->getFrameBuffer();
				layerFrameBuffer.bind();
				glReadPixels(int(mousePosInNTS.x * layerFrameBuffer.getTexture().getWidth()), int(mousePosInNTS.y * layerFrameBuffer.getTexture().getHeight()), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelColor);
				layerFrameBuffer.unbind();
				spdlog::info("Alpha : {}", pixelColor[3]);
				//
				if (pixelColor[3] > Settings::MIN_ALPHA_TO_GRAB_LAYER) {
					m_hoveredLayer = layers[k];
					break;
				}
			}
			// check if we're on a border
			else if (m_mousePosRelToHoveredLayer != OUTSIDE) {
				m_hoveredLayer = layers[k];
				break;
			}
		}
	}
}

bool LayerList::mustShowAltOrigin() {
	return Input::keyIsDown(ALT) && !selectedLayers.isEmpty();//|| getActivLayer()->m_transform.isDraggingRotation();
}
bool LayerList::mouseIsHoveringAltOrigin() {
	return mustShowAltOrigin() && glm::length(selectedLayers.getAltOriginInWindowSpace() - Input::getMousePosition()) < Settings::ALT_ORIGIN_RADIUS;
}
bool LayerList::canDragRotation() {
	if (mustShowAltOrigin()) {
		float distToAltOrigin = glm::length(selectedLayers.getAltOriginInWindowSpace() - Input::getMousePosition());
		return Settings::START_ROTATING_MIN_RADIUS < distToAltOrigin && distToAltOrigin < Settings::START_ROTATING_MAX_RADIUS;
	}
	else {
		return false;
	}
}

void LayerList::onKeyDown(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey)
		{
		case ALT:
			selectedLayers.changeDraggingCenterToAltOrigin();
			break;
		case CTRL:
			break;
		case SHIFT:
			break;
		case SPACE:
			break;
		default:
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		if (Input::keyIsDown(CTRL)) {
			switch (*c) {
			case 'd':
				for (int k = 0; k < selectedLayers.size(); ++k) {
					DrawingBoard::LayerRegistry().createCopyLayer(selectedLayers[k]);
				}
			default:
				break;
			}
		}
	}
}

void LayerList::onKeyUp(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey)
		{
		case ALT:
			selectedLayers.revertDraggingCenterToInitialOrigin();
			break;
		case CTRL:
			break;
		case SHIFT:
			break;
		case SPACE:
			break;
		default:
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char

	}
}

void LayerList::onDoubleLeftClic() {
	if (mouseIsHoveringAltOrigin()) {
		selectedLayers.resetAltOrigin();
	}
}

void LayerList::onLeftClicDown() {
	//Drag alt origin
	if (mouseIsHoveringAltOrigin()) {
		selectedLayers.startDraggingAltOrigin();
	}
	//Rotate
	else if (canDragRotation()) {
		selectedLayers.startDraggingRotation();
	}
	else if (m_hoveredLayer)
	{
		//Change selected layers
		if (Input::keyIsDown(CTRL)) {
			if( selectedLayers.contains(m_hoveredLayer))
				selectedLayers.removeLayerByRegisterID(m_hoveredLayer);
			else
				selectedLayers.addLayer(m_hoveredLayer);
		}
		else if (!selectedLayers.contains(m_hoveredLayer)) {
			setSelectedLayer(m_hoveredLayer);
		}
		//Translate
		if (m_mousePosRelToHoveredLayer == INSIDE) {
			selectedLayers.startDraggingTranslation();
		}
		//Drag scale/aspectRatio
		else {
			glm::vec2 originInTransformSpace;
			switch (m_mousePosRelToHoveredLayer)
			{
			case OUTSIDE:
				spdlog::error("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're outside any layer");
				break;
				//Drag translation if inside
			case INSIDE:
				spdlog::error("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're inside a layer");
				break;
				//Scale towards opposite border
			case RIGHT:
				originInTransformSpace = glm::vec2(-0.5f * DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getAspectRatio(), 0.0f);
				break;
			case TOP_RIGHT:
				originInTransformSpace = glm::vec2(-0.5f * DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getAspectRatio(), -0.5f);
				break;
			case TOP:
				originInTransformSpace = glm::vec2(0.0f, -0.5f);
				break;
			case TOP_LEFT:
				originInTransformSpace = glm::vec2(0.5f * DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getAspectRatio(), -0.5f);
				break;
			case LEFT:
				originInTransformSpace = glm::vec2(0.5f * DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getAspectRatio(), 0.0f);
				break;
			case BOT_LEFT:
				originInTransformSpace = glm::vec2(0.5f * DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getAspectRatio(), 0.5f);
				break;
			case BOT:
				originInTransformSpace = glm::vec2(0.0f, 0.5f);
				break;
			case BOT_RIGHT:
				originInTransformSpace = glm::vec2(-0.5f * DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getAspectRatio(), 0.5f);
				break;
			default:
				spdlog::error("[LayerList::onLeftClicDown] reached default case");
				break;
			}
			// Unlock the correct u and/or v aspect ratio in case we switch for dragging ratio
			bool unlockUForClickedLayer = (originInTransformSpace.x != 0.0f);
			bool unlockVForClickedLayer = (originInTransformSpace.y != 0.0f);
			// Start dragging
			selectedLayers.startDraggingScale(m_hoveredLayer, DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getMatrix() * glm::vec4(originInTransformSpace, 0.0f, 1.0f), DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getUAxis(), DrawingBoard::LayerRegistry()[m_hoveredLayer]->m_transform.getVAxis(), unlockUForClickedLayer, unlockVForClickedLayer);
			//Scale towards alt origin if ALT down
			if (Input::keyIsDown(ALT)) {
				selectedLayers.changeDraggingCenterToAltOrigin();
			}
		}
	}
	else {
		selectedLayers.removeAllLayers();
	}
}

void LayerList::onLeftClicUp() {
	if(m_hoveredLayer)
		selectedLayers.pushStateInHistoryAtTheEndOfDragging();
	selectedLayers.endDragging();
	usedCursor = nullptr;
}

void LayerList::onScroll(float motion) {
	if (motion < 0.0f) {
		selectedLayers.scale(Settings::ZOOM_FACTOR, true);
	}
	else {
		selectedLayers.scale(1.0f / Settings::ZOOM_FACTOR, true);
	}
}

void LayerList::setCursor() {
	if (isBusy() && usedCursor) {
		Cursor::set(usedCursor);
	}
	else {
		if (mouseIsHoveringAltOrigin()) {
			usedCursor = Cursor::fourDirections;
			Cursor::set(Cursor::fourDirections);
		}
		else if (canDragRotation()) {
			usedCursor = Cursor::rotate;
			Cursor::set(Cursor::rotate);
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
}