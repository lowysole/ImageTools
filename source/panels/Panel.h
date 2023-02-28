#pragma once

#include "float2.h"

class Panel {
public:
	Panel(const char* _name, bool _enabled);

	virtual void DrawPanel();

	bool& UpdateEnabled();
	const char* GetName() const;
	bool GetEnabled() const;

private:
	const char* name;
	bool enabled;
};
