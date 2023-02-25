#include "PanelResource.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

ResourceClass* PanelResource::resourceClassSelected = nullptr;

PanelResource::PanelResource()
	: Panel("Resource", true, float2(100.0f, 20.0f)) {
}

void PanelResource::Update() {
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + GetPos().x, viewport->WorkPos.y + GetPos().y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * GetSize().x, viewport->WorkSize.y * GetSize().y), ImGuiCond_FirstUseEver);

	//if (App->editor->GetIsResized()) {
	//	ImGuiViewport* viewport = ImGui::GetMainViewport();
	//	ImVec2 newSize = ImVec2(App->editor->GetWorkSize().x / viewport->WorkSize.x, App->editor->GetWorkSize().x / viewport->WorkSize.x);
	//	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + GetPos().x * newSize.x, viewport->WorkPos.y + GetPos().y * newSize.y));
	//	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * GetSize().x * newSize.x, viewport->WorkSize.y * GetSize().y * newSize.x));
	//}

	std::string windowName = std::string(ICON_FA_CUBE "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
		// Tab Bar
		const char* names[2] = {"Math", "Geometry"};

		if (ImGui::BeginTabBar("#resource_tab", ImGuiTabBarFlags_None)) {
			for (int n = 0; n < IM_ARRAYSIZE(names); ++n) {
				if (ImGui::BeginTabItem(names[n])) {
					switch (n) {
					case static_cast<int>(ResourceType::MATH):
						ImGui::NewLine();
						if (ImGui::Button("Float2")) resourceClassSelected = &resourceClassFloat2;
						ImGui::SameLine();
						if (ImGui::Button("Float3")) resourceClassSelected = nullptr;
						break;
					case static_cast<int>(ResourceType::GEOMETRY):
						break;
					}
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

ResourceClass* PanelResource::GetResourceClassSelected() {
	return resourceClassSelected;
}
