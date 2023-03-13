#include "Resource.h"

#include "Modules/ModuleTexture.h"
#include "Utils/ErrorLogs.h"
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
	resourceDataChannels[0] = new cv::Mat();

	if (panelResource->GetPanelResourceType() == PanelResourceType::COMPARE_IMAGE) {
		resourceData[1] = new cv::Mat();
		resourceData[2] = new cv::Mat();

		resourceDataChannels[1] = new cv::Mat();
		resourceDataChannels[2] = new cv::Mat();

		resourceName[2] = panelResource->GetName();
	}
}

Resource::~Resource() {
	DeleteResource(0);

	if (panelResource->GetPanelResourceType() == PanelResourceType::COMPARE_IMAGE) {
		DeleteResource(1);
		DeleteResource(2);
	}
}

bool Resource::ReadImage(const char* _filePath, const uint id) {
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

	filePath[id] = _filePath;
	resourceName[id] = GetFileName(_filePath);
	hasResource[id] = true;
	panelResource->SetHasChanged(true);

	if (panelResource->GetPanelResourceType() == PanelResourceType::IMAGE) {
		panelResource->SetName(resourceName[id].c_str());
	}

	resourceID[id] = ModuleTexture::CreateTexture(resourceData[id]);

	panelResource->EmptyErrorLogs();
	LOG("Image loaded: %s", resourceName[id].c_str());

	return true;
}

bool Resource::SaveImage(const char* _filePath, uint id) {
	if (!hasResource[id]) return false;

	cv::Mat image;
	switch (resourceData[id]->channels()) {
	case 3:
		cv::cvtColor(*resourceData[id], image, cv::COLOR_RGB2BGR);
		break;
	case 4:
		cv::cvtColor(*resourceData[id], image, cv::COLOR_RGBA2BGRA);
		break;
	default:
		break;
	}

	bool saved = cv::imwrite(_filePath, image);
	return saved;
}

void Resource::UpdateImage(const uint id) {
	if (!resourceData[id]) return;

	hasResource[id] = true;

	if (!resourceID[id]) {
		resourceID[id] = ModuleTexture::CreateTexture(resourceData[id]);
	}
	UpdateImageChannels(id, activeChannels, resourceData[id]->channels());
}

void Resource::UpdateImageChannels(const uint id, const bool* channels, const uint numChannels) {
	if (!hasResource[id]) return;

	std::vector<cv::Mat> channelsData;
	cv::split(*resourceData[id], channelsData);
	for (uint i = 0; i < numChannels; ++i) {
		if (!channels[i]) {
			channelsData[i] = cv::Mat::zeros(resourceData[id]->rows, resourceData[id]->cols, CV_8UC1);
		}
	}

	cv::merge(channelsData, *resourceDataChannels[id]);

	ModuleTexture::UpdateTexture(resourceDataChannels[id], resourceID[id]);
}

bool Resource::GenerateImageDiff() {
	bool hasError = false;
	if (resourceData[0]->size() != resourceData[1]->size()) {
		panelResource->AddErrorLog(ErrorLogType::ERROR, ErrorLogNumber::ERROR_1);
		hasError = true;
	}

	if (resourceData[0]->channels() != resourceData[1]->channels()) {
		panelResource->AddErrorLog(ErrorLogType::ERROR, ErrorLogNumber::ERROR_2);
		hasError = true;
	}

	if (ModuleTexture::GetTextureType(resourceData[0]->type()) != ModuleTexture::GetTextureType(resourceData[1]->type())) {
		panelResource->AddErrorLog(ErrorLogType::WARNING, ErrorLogNumber::WARNING_1);
	}

	if (hasError) {
		DeleteResource(2);
		resourceData[2] = new cv::Mat();
		resourceDataChannels[2] = new cv::Mat();
		return false;
	}

	ModuleTexture::GenerateDiffImage(GetResourceData(0), GetResourceData(1), GetResourceData(2), compareType, bitmaskThreshold);
	UpdateImage(2);

	errorPSN = ModuleTexture::CalculatePSN(GetResourceData(0), GetResourceData(1));

	return true;
}

bool Resource::HasResource(const uint id) const {
	return hasResource[id];
}

const char* Resource::GetResourceFilePath(const uint id) const {
	return filePath[id].c_str();
}

const char* Resource::GetResourceName(const uint id) const {
	return resourceName[id].c_str();
}

unsigned Resource::GetResourceID(const uint id) const {
	return resourceID[id];
}

cv::Mat* Resource::GetResourceData(const uint id) const {
	return resourceData[id];
}

bool* Resource::UpdateActiveChannels() {
	return &activeChannels[0];
}

void Resource::DeleteResource(const uint id) {
	ModuleTexture::DeleteTexture(resourceID[id]);
	RELEASE(resourceData[id]);
	RELEASE(resourceDataChannels[id]);

	hasResource[id] = false;
	LOG("Image closed: %s", resourceName[id].c_str());
}
