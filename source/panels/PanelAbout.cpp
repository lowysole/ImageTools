#include "PanelAbout.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"

#include "imgui.h"
#include "IconsForkAwesome.h"

#include <string>

#include "Utils/Leaks.h"


PanelAbout::PanelAbout()
	: Panel("About", false) {
}

void PanelAbout::DrawPanel() {
	std::string windowName = std::string(ICON_FK_INFO_CIRCLE "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled(), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
		ImGui::TextColored(App->editor->titleColor, "ImageTool");
		ImGui::Separator();
		ImGui::TextColored(App->editor->titleColor, u8"Lowy Solé");
		ImGui::Text( u8"https://github.com/lowysole/ImageTools");
		ImGui::Text("");
		ImGui::Text("Licensed under the MIT License.");
		ImGui::Separator();
	}
	ImGui::End();
}