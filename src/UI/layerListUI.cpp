#include "layerListUI.hpp"

#include "imgui/imgui.h"

#include "UI/input.hpp"

#include "core/drawingBoard.hpp"

void UI::showLayerList() {

	LayerList& layerList = DrawingBoard::getLayerList();

	ImGui::Begin("Layers"); 
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.95f);

	for (int k = 0; k < layerList.layers.size(); ++k) {
		Layer* layer = layerList.layers[k];
		ImGui::BeginChild(layer->getName().c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 150));
			if (ImGui::Selectable(layer->getName().c_str(), layerList.selectedLayers.contains(layer))) {
				if (Input::keyIsDown(CTRL)) {
					if (layerList.selectedLayers.contains(layer))
						layerList.selectedLayers.removeLayer(layer);
					else
						layerList.selectedLayers.addLayer(layer);
				}
				else {
					layerList.setSelectedLayer(layer);
				}
			}
			ImGui::Image((ImTextureID)layer->getTexture().getID(), ImVec2(50 * layer->getTexture().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
			ImGui::Checkbox("visible", layerList.getLayer(k)->getIsVisiblePointer());
			if (ImGui::Button("Reset Transform")) {
				layer->m_transform.reset();
			}
		ImGui::EndChild();
		ImGui::Separator();
	}
	ImGui::PopItemWidth();
	ImGui::End();

}