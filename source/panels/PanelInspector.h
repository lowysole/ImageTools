#pragma once

#include "Panels/Panel.h"

class PanelResource;
class PanelInspector : public Panel {
public:
	PanelInspector();

	void DrawPanel() override;

	void DrawPanelInspectorDefault();
	void DrawPanelInspectorImage();
	void DrawPanelInspectorCompare();

	PanelResource* GetPanelResourceSelected() const;
	void SetPanelResourceSelected(PanelResource* _panelResourceSelected);

private:
	PanelResource* panelResourceSelected = nullptr;
};
