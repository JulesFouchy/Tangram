#include "layerListUI.hpp"

#include "imgui/imgui.h"

#include "UI/input.hpp"
#include "UI/log.hpp"

#include "core/drawingBoard.hpp"

void UI::showLayerList() {

	LayerList& layerList = DrawingBoard::getLayerList();

	ImGui::Begin("Layers"); 
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.95f);

	ImGui::Text("-------------------------");
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Reordering layer"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Layer*));
			Layer* payload_layer = *(Layer * *)payload->Data;
			layerList.layers.reorderLayer(payload_layer, layerList.layers.size());
		}
		ImGui::EndDragDropTarget();
	}
	for (int k = layerList.layers.size() - 1; k >= 0 ; --k) {
		Layer* layer = layerList.layers[k];
		//
		//ImGui::BeginChild(layer->getName().c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 150));
			ImGui::PushID((int)layer);
			ImGui::Button("Drag me !");
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("Reordering layer", &layer, sizeof(Layer*));
				ImGui::Image((ImTextureID)layer->getTexture().getID(), ImVec2(50 * layer->getTexture().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
				ImGui::EndDragDropSource();
			}
			// onClic : add/remove layer from selection
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
			// miniature
			ImGui::Image((ImTextureID)layer->getTexture().getID(), ImVec2(50 * layer->getTexture().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
			// visibiliy checkbox
			ImGui::Checkbox("visible", layerList.getLayer(k)->getIsVisiblePointer());
			// reset transform
			if (ImGui::Button("Reset Transform")) {
				layer->m_transform.reset();
			}
			ImGui::PopID();
		//ImGui::EndChild();
			ImGui::Text("-------------------------");
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Reordering layer"))
				{
					IM_ASSERT(payload->DataSize == sizeof(Layer*));
					Layer* payload_layer = *(Layer * *)payload->Data;
					layerList.layers.reorderLayer(payload_layer, k);
				}
				ImGui::EndDragDropTarget();
			}
		//
	}
	ImGui::PopItemWidth();
	ImGui::End();

}