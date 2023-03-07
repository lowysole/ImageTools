#pragma once

#include "Panels/Panel.h"

class Resource;

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

private:

	void DrawPanelDefault();
	void DrawPanelImage();
	void DrawPanelCompare();
	void DrawImageTable(const char* name);

private:
	PanelResourceType panelResourceType;

	Resource* resource = nullptr;
};
