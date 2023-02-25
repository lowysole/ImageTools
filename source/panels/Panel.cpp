#include "Panel.h"

#include "imgui.h"

Panel::Panel(const char* _name, bool _enabled, float2 _pos, float2 _size)
	: name {_name}
	, enabled {_enabled}
	, pos {_pos}
	, size {_size} {
}

void Panel::Update() {}

bool& Panel::UpdateEnabled() {
	return enabled;
}

const char* Panel::GetName() const {
	return name;
}

bool Panel::GetEnabled() const {
	return enabled;
}

float2 Panel::GetPos() const {
	return pos;
}

float2 Panel::GetSize() const {
	return size;
}
