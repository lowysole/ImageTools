#pragma once


#include "opencv2/core/mat.hpp"
#include "Panels/PanelResource.h"

#include <string>

class Resource {


public:

	Resource(PanelResource* _panelResource);
	~Resource();

	bool ReadImage(const char* filePath, uint id);

	void UpdateImage(const uint id, const bool* channels, const uint numChannels);

	bool HasResource(const uint id) const;

	const char* GetResourceFilePath(const uint id) const;
	const char* GetResourceName(const uint id) const;

	unsigned GetResourceID(const uint id) const;
	cv::Mat* GetResourceData(const uint id) const;

	bool* UpdateActiveChannels();

	void DeleteResource(uint id);


private:
	std::string filePath[3] = {"", "", ""};
	std::string resourceName[3] = {"", "", ""};

	uint resourceID[3] = {0, 0, 0};
	cv::Mat* resourceData[3] = {nullptr, nullptr, nullptr};
	cv::Mat* resourceDataChannels[3] = {nullptr, nullptr, nullptr};
	bool activeChannels[4] = {true, true, true, true};
	PanelResource* panelResource;

	bool hasResource[3] = {false, false, false};


};
