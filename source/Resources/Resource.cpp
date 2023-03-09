#include "Resource.h"

#include "Modules/ModuleTexture.h"
#include "Utils/FileSystem.h"
#include "Utils/Logging.h"

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <string>

#include "Utils/Leaks.h"

Resource::Resource(PanelResource* _panelResource) {
	panelResource = _panelResource;
	resourceData[0] = new cv::Mat();
	resourceData[1] = new cv::Mat();
	resourceData[2] = new cv::Mat();
}

Resource::~Resource() {
	DeleteResource(0);
	DeleteResource(1);
	DeleteResource(2);
}

bool Resource::ReadImage(const char* _filePath, uint id) {
	if (!cv::haveImageReader(_filePath)) {
		LOG("Cannot open this file format")
		filePath[id] = "";
		return false;
	}

	cv::Mat image = cv::imread(_filePath);
	if (image.empty()) {
		LOG("Image data is corrupted")
		filePath[id] = "";
		return false;
	}

	switch (image.channels()) {
	case 3:
		cv::cvtColor(image, *resourceData[id], cv::COLOR_BGR2RGB);
		break;
	case 4:
		cv::cvtColor(image, *resourceData[id], cv::COLOR_BGRA2RGBA);
		break;
	default:
		break;
	}

	// Split channels data
	//std::vector<cv::Mat> channelsData;
	//cv::split(*resourceData[id], channelsData);

	//for (int i = 0; i < resourceData[id]->channels(); ++i) {
	//	resourceDataChannels.push_back(new cv::Mat());
	//}

	//int i = 0;
	//for (auto channel : resourceDataChannels) {
	//	*channel = channelsData[i].clone();
	//	++i;
	//}

	filePath[id] = _filePath;
	resourceName[id] = GetFileName(_filePath);
	hasResource[id] = true;

	if (panelResource->GetPanelResourceType() == PanelResourceType::IMAGE) {
		panelResource->SetName(resourceName[id].c_str());
	}

	resourceID[id] = ModuleTexture::CreateTexture(resourceData[id]);

	LOG("Image loaded: %s", resourceName[id].c_str());

	return true;
}

bool Resource::HasResource(uint id) const {
	return hasResource[id];
}

const char* Resource::GetResourceFilePath(uint id) const {
	return filePath[id].c_str();
}

const char* Resource::GetResourceName(uint id) const {
	return resourceName[id].c_str();
}

unsigned Resource::GetResourceID(uint id) const {
	return resourceID[id];
}

cv::Mat* Resource::GetResourceData(uint id) {
	return resourceData[id];
}

void Resource::DeleteResource(uint id) {
	ModuleTexture::DeleteTexture(resourceID[id]);
	RELEASE(resourceData[id]);

	for (auto channel : resourceDataChannels) {
		RELEASE(channel)
	}

	LOG("Image closed: %s", resourceName[id].c_str());
}
