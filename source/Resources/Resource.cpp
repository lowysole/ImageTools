#include "Resource.h"

#include "Utils/FileSystem.h"
#include "Utils/Logging.h"

#include "GL/glew.h"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <string>

#include "Utils/Leaks.h"

Resource::Resource(PanelResource* _panelResource) {
	panelResource = _panelResource;
	resourceData = new cv::Mat();
}

Resource::~Resource() {
	RELEASE(resourceData);
}

bool Resource::ReadImage(const char* _filePath) {
	if (!cv::haveImageReader(_filePath)) {
		LOG("Cannot open this file format")
		filePath = "";
		return false;
	}

	cv::Mat image = cv::imread(_filePath);
	if (image.empty()) {
		LOG("Image data is corrupted")
		filePath = "";
		return false;
	}
	image.convertTo(*resourceData, CV_32F);

	cv::cvtColor(*resourceData, *resourceData, cv::COLOR_BGR2RGBA);

	filePath = _filePath;
	resourceName = GetFileName(_filePath);
	hasResource = true;

	panelResource->SetName(resourceName.c_str());

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	GLenum format;
	switch (resourceData->channels()) {
	case 1:
		format = GL_R;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;

	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, resourceData->cols, resourceData->rows, 0, format, GL_FLOAT, resourceData->data);

	resourceID = texture;

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

unsigned Resource::GetResourceID() const {
	return resourceID;
}

cv::Mat* Resource::GetResourceData(){
	return resourceData;
}
