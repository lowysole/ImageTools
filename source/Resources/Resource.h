#pragma once


#include "opencv2/core/mat.hpp"
#include "Panels/PanelResource.h"

#include <string>

class Resource {


public:

	Resource(PanelResource* _panelResource);
	~Resource();

	bool ReadImage(const char* filePath);

	bool HasResource() const;

	const char* GetResourceFilePath() const;
	const char* GetResourceName() const;

	unsigned GetResourceID() const;
	cv::Mat* GetResourceData();


private:
	std::string filePath = "";
	std::string resourceName = "";

	unsigned resourceID = 0;
	cv::Mat* resourceData = nullptr;
	PanelResource* panelResource;

	bool hasResource = false;

};
