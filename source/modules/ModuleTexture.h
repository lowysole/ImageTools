#pragma once

#include "opencv2/core.hpp"

#include <string>

enum class CompareType {
	BITMASK,
	FOREGROUND
};

class ModuleTexture {
public:

	static uint CreateTexture(cv::Mat* resource);
	static void UpdateTexture(cv::Mat* resource, uint textureId);
	static void GenerateDiffImage(cv::Mat* resOriginal, cv::Mat* resCompare, cv::Mat* resOutput, CompareType compareType, float threshold);

	static double CalculatePSN(cv::Mat* resOriginal, cv::Mat* resCompare);

	static uint GetTextureFormat(int channels);
	static std::string GetTextureFormatToString(int channels);
	static uint GetTextureGLType(int type);
	static uchar GetTextureType(int type);
	static std::string GetTextureTypeToString(int type);

	static void DeleteTexture(uint &glTexture);
};