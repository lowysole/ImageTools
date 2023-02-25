#pragma once

#include "Panel.h"

#include "ResourceClass/ResourceClass.h"
#include "ResourceClass/Math/ResourceClassFloat2.h"

enum class ResourceType {
	MATH,
	GEOMETRY,
};

class PanelResource : public Panel {
public:
	PanelResource();

	void Update() override;

	static ResourceClass* GetResourceClassSelected();

public:
	static ResourceClass* resourceClassSelected;

private:
	// Resource Classes
	ResourceClassFloat2 resourceClassFloat2;
};
