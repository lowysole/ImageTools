#include "PanelOptions.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Panels/PanelResource/PanelImage.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "float2.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

PanelOptions::PanelOptions()
	: Panel("Options", true) {
}

void PanelOptions::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockRightId, ImGuiCond_FirstUseEver);
	
	std::string windowName = std::string(ICON_FA_CALCULATOR "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
		ImGui::NewLine();

		ImGui::Text("Select a resource from Resource windows");
	}
	ImGui::End();
}
