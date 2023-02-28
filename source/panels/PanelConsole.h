#pragma once

#include "Panels/Panel.h"

class PanelConsole : public Panel {
public:
	PanelConsole();

	void DrawPanel() override;
};
