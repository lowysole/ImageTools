#pragma once

#include "opencv2/core/mat.hpp"

#include <string>

class ModuleTexture {
public:

	static uint CreateTexture(cv::Mat* resource);
	static void UpdateTexture(cv::Mat* resource, uint textureId);
	static uint GetTextureFormat(int channels);
	static std::string GetTextureFormatToString(int channels);
	static uint GetTextureType(int type);
	static std::string GetTextureTypeToString(int type);

	

	static void DeleteTexture(uint &glTexture);
};