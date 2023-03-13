#pragma once

#include <string>
#include <iostream>

static std::string GetDirPath(const char* filePath, const char* dSlash = "/\\") {
	size_t indexSlash = std::string(filePath).find_last_of(dSlash);
	return std::string(filePath).substr(0, indexSlash + 1);
}

static std::string GetFileName(const char* filePath, const char* dSlash = "/\\") {
	size_t indexSlash = std::string(filePath).find_last_of(dSlash);
	return std::string(filePath).substr(indexSlash + 1);
}

static std::string GetFileNameNoExtension(const char* fileName) {
	size_t index_of_period = std::string(fileName).find_last_of('.');
	return std::string(fileName).substr(0, index_of_period).c_str();
}