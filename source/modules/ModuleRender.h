#pragma once

#include "Modules/Module.h"

#include "Resources/Math/float2.h"

class ModuleRender : public Module {
public:
	bool Init() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;

	bool IsResized();

	bool GetIsResized() const;
	float2 GetViewportSize() const;

public:
	void* context = nullptr; // SDL context

private:
	// ------- Viewport ------- //
	bool viewportUpdated = true;
	float2 viewportSize = float2::zero;
};
