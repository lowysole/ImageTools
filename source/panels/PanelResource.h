#pragma once

#include "Panels/Panel.h"

#include "float2.h"

#include <vector>
#include <string>

class Resource;
enum class ErrorLogType;
enum class ErrorLogNumber;

typedef unsigned uint;

enum class PanelResourceType {
	NONE,
	IMAGE,
	COMPARE_IMAGE,
};

class PanelResource : public Panel {
public:
	PanelResource();
	PanelResource(const char* _resourceName, PanelResourceType _panelResourceType);
	~PanelResource();

	void DrawPanel();

	PanelResourceType GetPanelResourceType() const;
	Resource* GetResource() const;
	bool GetHasChanged() const;
	void SetHasChanged(bool _hasChanged);

	void AddErrorLog(ErrorLogType errorType, ErrorLogNumber errorNumber);
	void EmptyErrorLogs();

	void DeletePanel() override;

private:
	void DrawPanelDefault();
	void DrawPanelImage();
	void DrawPanelCompare();
	void DrawImageTable(const char* name, uint id);

private:
	PanelResourceType panelResourceType;

	Resource* resource = nullptr;

	bool hasChanged = false;

	// ImGui Panel parameters
	float2 infoTableSize = {200.0f, 20.0f};

	bool errorLogsOpened = false;
	std::vector<std::string> errorLogs;

};
