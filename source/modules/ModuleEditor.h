#pragma once

#include "Module.h"

#include "Panels/Panel.h"
#include "Panels/PanelAbout.h"
#include "Panels/PanelInspector.h"
#include "Panels/PanelConsole.h"
#include "Panels/PanelResource.h"

#include "imgui.h"
#include <vector>

class float2;

enum class Modal {
	NONE,
	EXPORT,
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

	void CreatePanelResource(PanelResourceType resourceType);

public:
	// ---------- Docking ----------
	unsigned dockMainId = 0;
	unsigned dockRightId = 0;
	unsigned dockDownId = 0;

	// Panels
	std::vector<Panel*> panels;
	PanelAbout panelAbout;
	PanelInspector panelInspector;
	PanelConsole panelConsole;
	PanelResource panelResource = PanelResource();

	std::vector<Panel*> panelsResource;
	

	// ImGui parameters
	const ImVec4 titleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

private:
	void SetImGuiTheme();

private:
	Modal modalToOpen = Modal::NONE;

	bool isResized = false;
	float2 workPos = float2::zero;
	float2 workSize = float2::zero;
};
