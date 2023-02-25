#include "PanelOperations.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Panels/PanelResource.h"
#include "ResourceClass/ResourceClass.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

PanelOperations::PanelOperations()
	: Panel("Operations", true, float2(20.0f, 20.0f), float2(0.2f, 0.75f)) {
}

void PanelOperations::Update() {
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + GetPos().x, viewport->WorkPos.y + GetPos().y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * GetSize().x, viewport->WorkSize.y * GetSize().y), ImGuiCond_FirstUseEver);

	//if (App->editor->GetIsResized()) {
	//	ImGuiViewport* viewport = ImGui::GetMainViewport();
	//	ImVec2 newSize = ImVec2(App->editor->GetWorkSize().x / viewport->WorkSize.x, App->editor->GetWorkSize().x / viewport->WorkSize.x);
	//	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + GetPos().x * newSize.x, viewport->WorkPos.y + GetPos().y * newSize.y));
	//	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * GetSize().x * newSize.x, viewport->WorkSize.y * GetSize().y * newSize.x));
	//}

	std::string windowName = std::string(ICON_FA_CALCULATOR "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
		ImGui::NewLine();
		ResourceClass* resourceClassSelected = PanelResource::GetResourceClassSelected();
		if (resourceClassSelected) {
			resourceClassSelected->DrawOnPanel();
		} else {
			ImGui::Text("Select a resource from Resource windows");
		}
	}
	ImGui::End();
}
