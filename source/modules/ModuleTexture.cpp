#include "ModuleTexture.h"

#include "GL/glew.h"

#include "Utils/Logging.h"

typedef unsigned uint;

uint ModuleTexture::CreateTexture(cv::Mat* resource) {

	GLenum format = GetTextureFormat(resource->channels());
	GLenum type = GetTextureType(resource->type());

	if ((format || type) == 0) {
		return 0;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, format, resource->cols, resource->rows, 0, format, type, resource->data);

	return texture;
}

void ModuleTexture::UpdateTexture(cv::Mat* resource, uint textureId) {
	GLenum format = GetTextureFormat(resource->channels());
	GLenum type = GetTextureType(resource->type());

	if ((format || type) == 0) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, resource->cols, resource->rows, 0, format, type, resource->data);
}

uint ModuleTexture::GetTextureFormat(int channels) {
	switch (channels) {
	case 1:
		return GL_R;
	case 2:
		return GL_RG;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	}
	LOG("Incorrect texture format");
	return 0;
}

std::string ModuleTexture::GetTextureFormatToString(int channels) {
	switch (channels) {
	case 1:
		return std::string("R");
	case 2:
		return std::string("RG");
	case 3:
		return std::string("RGB");
	case 4:
		return std::string("RGBA");
	}
	LOG("Incorrect texture format");
	return "";
}

uint ModuleTexture::GetTextureType(int type) {
	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:
		return GL_UNSIGNED_BYTE;
	case CV_8S:
		return GL_BYTE;
	case CV_16U:
		return GL_SHORT;
	case CV_16S:
		return GL_SHORT;
	case CV_32S:
		return GL_FLOAT;
	case CV_32F:
		return GL_FLOAT;
	case CV_64F:
		return GL_DOUBLE;
	}
	LOG("Incorrect texture type");
	return 0;
}

std::string ModuleTexture::GetTextureTypeToString(int type) {
	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:
		return std::string("8U");
	case CV_8S:
		return std::string("8S");
	case CV_16U:
		return std::string("16U");
	case CV_16S:
		return std::string("16U");
	case CV_32S:
		return std::string("32S");
	case CV_32F:
		return std::string("32F");
	case CV_64F:
		return std::string("64F");
	}
	LOG("Incorrect texture type");
	return "";
}

void ModuleTexture::DeleteTexture(uint &glTexture) {
		if (glTexture) {
			glDeleteTextures(1, &glTexture);
			glTexture = 0;
		}
}
