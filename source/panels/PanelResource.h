#pragma once

#include "Panels/Panel.h"

class Resource;

typedef unsigned uint;

enum class PanelResourceType {
	NONE,
	IMAGE,
	COMPARE_IMAGE,
};

class PanelResource : public Panel {

public:

	PanelResource();
	PanelResource(const char* _resourceName, PanelResourceType _panelResourceType);
	~PanelResource();

	void DrawPanel();

	Resource* GetResource() const;

	PanelResourceType GetPanelResourceType() const;

	void DeletePanel() override;

private:

	void DrawPanelDefault();
	void DrawPanelImage();
	void DrawPanelCompare();
	void DrawImageTable(const char* name, uint id);

private:
	PanelResourceType panelResourceType;

	Resource* resource = nullptr;
};
