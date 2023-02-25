#pragma once

#include "Resources/Math/float2.h"
class Panel {
public:
	Panel(const char* _name, bool _enabled, float2 _pos = float2::zero, float2 _size = float2::one);

	virtual void Update();

	bool& UpdateEnabled();
	const char* GetName() const;
	bool GetEnabled() const;

	float2 GetPos() const;
	float2 GetSize() const;

private:
	const char* name;
	bool enabled;

	float2 pos = float2::zero;
	float2 size = float2::zero;
};
