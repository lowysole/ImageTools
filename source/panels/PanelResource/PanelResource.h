#pragma once

#include "Panels/Panel.h"
#include "Resources/Resource.h"

enum class PanelResourceType {
	NONE,
	IMAGE,
	COMPARE_IMAGE,
};

class PanelResource : public Panel {

public:

	PanelResource();
	PanelResource(const char* _resourceName, PanelResourceType _panelResourceType);

	virtual void DrawPanel();

	const char* GetPanelName();
	void SetPanelName();

	Resource* GetResource();
	void SetResource(Resource* resource);

private:

	const char* panelName = nullptr;
	PanelResourceType panelResourceType;

	Resource* resource = nullptr;
};
