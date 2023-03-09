#include "PanelInspector.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Resources/Resource.h"
#include "Panels/PanelResource.h"

#include "Utils/Logging.h"
#include "Utils/portable-file-dialogs.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

PanelInspector::PanelInspector()
	: Panel("Inspector", true) {
}

void PanelInspector::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockRightId, ImGuiCond_FirstUseEver);

	std::string windowName = std::string(ICON_FA_IMAGE "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
		switch (panelResourceSelected->GetPanelResourceType()) {
		case PanelResourceType::NONE:
			DrawPanelInspectorDefault();
			break;
		case PanelResourceType::IMAGE:
			DrawPanelInspectorImage();
			break;
		case PanelResourceType::COMPARE_IMAGE:
			DrawPanelInspectorImage();
			break;
		}

		ImGui::End();
	}
}

void PanelInspector::DrawPanelInspectorDefault() {
	ImGui::Text("PanelInspectorDefault");
}

void PanelInspector::DrawPanelInspectorImage() {
	Resource* resource = panelResourceSelected->GetResource();

	if (ImGui::BeginTabBar("##image_tabs", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Options")) {
			ImGui::NewLine();
			char name[50];
			strncpy_s(name, resource->GetResourceName(0), 50);
			ImGui::Text("Image");
			ImGui::InputText("##image_1", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_ReadOnly);
			if (ImGui::Button("Load Image")) {
				auto f = pfd::open_file("Choose files to read", pfd::path::home(), {"Image Files (.png .jpeg)", "*.png *.jpeg", "All Files", "*"}, pfd::opt::none).result();
				resource->ReadImage(f[0].c_str(), 0);
			}
			ImGui::NewLine();

			if (panelResourceSelected->GetPanelResourceType() == PanelResourceType::COMPARE_IMAGE) {
				char name[50];
				strncpy_s(name, resource->GetResourceName(1), 50);
				ImGui::Text("Image to compare");
				ImGui::InputText("##image_2", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_ReadOnly);
				if (ImGui::Button("Load Image##image_2")) {
					auto f = pfd::open_file("Choose files to read", pfd::path::home(), {"Image Files (.png .jpeg)", "*.png *.jpeg", "All Files", "*"}, pfd::opt::none).result();
					resource->ReadImage(f[0].c_str(), 1);
				}
				ImGui::NewLine();
			}

			ImGui::Separator();
			if (resource->HasResource(0) || resource->HasResource(1)) {
				ImGui::NewLine();
				ImGui::TextColored(App->editor->titleColor, "Channels");
				bool changed = false;
				changed |= ImGui::Checkbox("R", &chR);
				ImGui::SameLine();
				changed |= ImGui::Checkbox("G", &chG);
				ImGui::SameLine();
				changed |= ImGui::Checkbox("B", &chB);
				ImGui::SameLine();
				changed |= ImGui::Checkbox("A", &chA);

				if (changed) {
					// TODO : Apply modification
				}
			}


			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Modify")) {
			ImGui::Text("In progress...");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void PanelInspector::DrawPanelInspectorCompare() {
	ImGui::Text("PanelInspectorCompare");
}

PanelResource* PanelInspector::GetPanelResourceSelected() const {
	return panelResourceSelected;
}

void PanelInspector::SetPanelResourceSelected(PanelResource* _panelResourceSelected) {
	panelResourceSelected = _panelResourceSelected;
}
