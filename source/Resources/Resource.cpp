#include "Resource.h"

#include "Utils/FileSystem.h"
#include "Utils/Logging.h"

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"

#include <string>

#include "Utils/Leaks.h"

Resource::Resource(PanelResource* _panelResource) {
	panelResource = _panelResource;
}

bool Resource::ReadImage(const char* _filePath) {
	//if (!cv::haveImageReader(_filePath)) {
	//	LOG("Cannot open this file format")
	//	filePath = "";
	//	return false;
	//}

	//resourceData = cv::imread(_filePath);
	filePath = _filePath;
	resourceName = GetFileName(_filePath);
	hasResource = true;

	panelResource->SetName(resourceName.c_str());

	LOG("Image loaded: %s", resourceName.c_str());

	return true;
}

bool Resource::HasResource() const {
	return hasResource;
}

const char* Resource::GetResourceFilePath() const {
	return filePath.c_str();
}

const char* Resource::GetResourceName() const {
	return resourceName.c_str();
}

cv::Mat* Resource::GetResourceData() {
	return &resourceData;
}
