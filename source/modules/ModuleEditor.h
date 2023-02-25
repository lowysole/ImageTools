#pragma once

#include "Module.h"

#include "Panels/Panel.h"
#include "Panels/PanelAbout.h"
#include "Panels/PanelOperations.h"
#include "Panels/PanelResource.h"
#include "Panels/PanelResult.h"
#include "Panels/PanelConsole.h"

#include "imgui.h"
#include <vector>

enum class Modal {
	NONE,
	QUIT
};

class ModuleEditor : public Module {
public:
	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;

	bool GetIsResized() const;
	ImVec2 GetWorkSize() const;

public:
	// Panels
	std::vector<Panel*> panels;
	PanelAbout panelAbout;
	PanelOperations panelOperations;
	PanelResource panelResource;
	PanelResult panelResult;
	PanelConsole panelConsole;

	// ImGui parameters
	const ImVec4 titleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

private:
	void SetImGuiTheme();

private:
	Modal modalToOpen = Modal::NONE;

	bool isResized = false;
	ImVec2 workPos = ImVec2(0.0f, 0.0f);
	ImVec2 workSize = ImVec2(0.0f, 0.0f);
};
