#pragma once

#include "PanelResource.h"

class PanelImage : public PanelResource {
public:
	PanelImage(const char* panelName);

	void DrawPanel() override;

};
