#pragma once


#include "opencv2/core/mat.hpp"
#include "Panels/PanelResource.h"

#include <string>

class Resource {


public:

	Resource(PanelResource* _panelResource);
	~Resource();

	bool ReadImage(const char* filePath, uint id);

	bool HasResource(uint id) const;

	const char* GetResourceFilePath(uint id) const;
	const char* GetResourceName(uint id) const;

	unsigned GetResourceID(uint id) const;
	cv::Mat* GetResourceData(uint id);

	void DeleteResource(uint id);


private:
	std::string filePath[3] = {"", "", ""};
	std::string resourceName[3] = {"", "", ""};

	uint resourceID[3] = {0, 0, 0};
	cv::Mat* resourceData[3] = {nullptr, nullptr, nullptr};
	std::vector<cv::Mat*> resourceDataChannels;

	PanelResource* panelResource;

	bool hasResource[3] = {false, false, false};

};
