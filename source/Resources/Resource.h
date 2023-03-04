#pragma once


#include "opencv2/core/mat.hpp"
#include "Panels/PanelResource.h"

#include <string>

class Resource {


public:

	Resource(PanelResource* _panelResource);

	bool ReadImage(const char* filePath);

	const char* GetResourceFilePath() const;
	const char* GetResourceName() const;
	cv::Mat* GetResourceData();

private:
	std::string filePath = "";
	std::string resourceName = "";
	cv::Mat resourceData;
	PanelResource* panelResource;

};
