#pragma once
#include "PanelResource.h"

class PanelCompare : public PanelResource {
public:
	PanelCompare(const char* fileName);

	void DrawPanel() override;

};