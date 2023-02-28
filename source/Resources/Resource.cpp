#include "Resource.h"

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"

#include "Utils/Logging.h"
#include "Utils/FileSystem.h"

bool Resource::ReadImage(const char* _filePath) {
	filePath = _filePath;
	resourceName = GetFileNameNoExtension(GetFileName(filePath));

	if (!cv::haveImageReader(filePath)) {
		LOG("Cannot open this file format")
		return false;
	}

	resourceData = cv::imread(filePath);

	LOG("Image loaded: %s", GetFileName(filePath));
	return true;
}

const char* Resource::GetResourceName() {
	return resourceName;
}

cv::Mat* Resource::GetResourceData() {
	return &resourceData;
}
