#include "Resource.h"

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"

#include "Utils/Logging.h"
#include "Utils/FileSystem.h"

#include <string>

Resource::Resource(PanelResource* _panelResource) {
	panelResource = _panelResource;
}

bool Resource::ReadImage(const char* _filePath) {
	//if (!cv::haveImageReader("C:\Users\lowys\Pictures\Mems\image.png")) {
	//	LOG("Cannot open this file format")
	//	filePath = "";
	//	return false;
	//}

	//resourceData = cv::imread(name.str());
	filePath = _filePath;
	resourceName = GetFileName(_filePath);
	panelResource->SetName(resourceName.c_str());

	LOG("Image loaded: %s", resourceName.c_str());

	return true;
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
