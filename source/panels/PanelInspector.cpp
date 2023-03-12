#include "PanelInspector.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Modules/ModuleTexture.h"
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
		case PanelResourceType::IMAGE:
			DrawPanelInspectorImage();
			break;
		case PanelResourceType::COMPARE_IMAGE:
			DrawPanelInspectorImage();
			break;
		default:
			DrawPanelInspectorDefault();
		}

		ImGui::End();
	}
}

void PanelInspector::DrawPanelInspectorDefault() {
	ImGui::Text("Select one resource");
}

void PanelInspector::DrawPanelInspectorImage() {
	Resource* resource = panelResourceSelected->GetResource();
	PanelResourceType panelType = panelResourceSelected->GetPanelResourceType();

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

			if (panelType == PanelResourceType::COMPARE_IMAGE && resource->HasResource(0)) {
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

			if (panelResourceSelected->GetHasChanged() && resource->HasResource(0) && resource->HasResource(1)) {
				resource->GenerateImageDiff();
				panelResourceSelected->SetHasChanged(false);
			}

			ImGui::Separator();
			if (resource->HasResource(0) || resource->HasResource(1)) {
				ImGui::NewLine();
				ImGui::TextColored(App->editor->titleColor, "Channels");
				uint numChannels = resource->GetResourceData(0)->channels();
				bool* channels = resource->UpdateActiveChannels();
				bool changed = false;
				switch (numChannels) {
				case 1:
					changed |= ImGui::Checkbox("R", &channels[0]);
					break;
				case 2:
					changed |= ImGui::Checkbox("R", &channels[0]);
					ImGui::SameLine();
					changed |= ImGui::Checkbox("G", &channels[1]);
					break;
				case 3:
					changed |= ImGui::Checkbox("R", &channels[0]);
					ImGui::SameLine();
					changed |= ImGui::Checkbox("G", &channels[1]);
					ImGui::SameLine();
					changed |= ImGui::Checkbox("B", &channels[2]);
					break;
				case 4:
					changed |= ImGui::Checkbox("R", &channels[0]);
					ImGui::SameLine();
					changed |= ImGui::Checkbox("G", &channels[1]);
					ImGui::SameLine();
					changed |= ImGui::Checkbox("B", &channels[2]);
					ImGui::SameLine();
					changed |= ImGui::Checkbox("A", &channels[3]);
					break;
				default:
					break;
				}

				if (changed) {
					resource->UpdateImageChannels(0, &channels[0], numChannels);

					if (panelType == PanelResourceType::COMPARE_IMAGE) {
						resource->UpdateImageChannels(1, &channels[0], numChannels);
						resource->UpdateImageChannels(2, &channels[0], numChannels);
					}
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
