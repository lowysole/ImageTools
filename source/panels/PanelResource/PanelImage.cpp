#include "PanelImage.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"


PanelImage::PanelImage(const char* panelName)
	: PanelResource(panelName, PanelResourceType::IMAGE) {
}

void PanelImage::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockMainId, ImGuiCond_FirstUseEver);

	std::string windowName = std::string(ICON_FA_IMAGE "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
	}
	ImGui::End();
}
