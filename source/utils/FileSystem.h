#pragma once

#include <string>
#include <iostream>
using namespace std;

const char* GetFileName(const char* filePath, const char* dSlash = "/\\") {
	size_t indexSlash = std::string(filePath).find_last_of(dSlash);
	return std::string(filePath).substr(indexSlash + 1).c_str();
}

const char* GetFileNameNoExtension(const char* fileName) {
	size_t index_of_period = std::string(fileName).find_last_of('.');
	return std::string(fileName).substr(0, index_of_period).c_str();
}