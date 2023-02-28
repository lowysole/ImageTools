#pragma once


#include "opencv2/core/mat.hpp"

class Resource {


public:
	
	const char* GetResourceName();
	cv::Mat* GetResourceData();

private:
	bool ReadImage(const char* filePath);

private:
	const char* filePath = nullptr;
	const char* resourceName = nullptr;
	cv::Mat resourceData;

};
