#pragma once

#include <string>

class Panel {
public:
	Panel(const char* _name, bool _enabled);

	virtual void DrawPanel();
	virtual void DeletePanel();
	
	const char* GetName() const;
	void SetName(const char* _name);

	bool& UpdateEnabled();
	bool GetEnabled() const;
	void SetEnabled(bool _enabled);


private:
	std::string name;
	bool enabled;
};
