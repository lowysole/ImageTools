#include "Panel.h"

Panel::Panel(const char* _name, bool _enabled)
	: name {_name}
	, enabled {_enabled} {
}

void Panel::DrawPanel() {}

const char* Panel::GetName() const {
	return name.c_str();
}

void Panel::SetName(const char* _name) {
	name = _name;
}

bool& Panel::UpdateEnabled() {
	return enabled;
}

bool Panel::GetEnabled() const {
	return enabled;
}

void Panel::SetEnabled(bool _enabled) {
	enabled = _enabled;
}
