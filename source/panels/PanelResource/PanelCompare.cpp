#include "PanelCompare.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

PanelCompare::PanelCompare(const char* fileName)
	: PanelResource(fileName, PanelResourceType::COMPARE_IMAGE) {
}

void PanelCompare::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockMainId, ImGuiCond_FirstUseEver);

	std::string windowName = std::string(ICON_FA_IMAGES "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
	}
	ImGui::End();
}