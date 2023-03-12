#pragma once

enum class ErrorLogType {
	ERROR,
	WARNING
};

// Add Error or Warning number and text to display in the next statements:
enum class ErrorLogNumber { 
	// Error
	ERROR_1,
	ERROR_2,

	// Warning
	WARNING_1,
};

constexpr const char* ErrorLogToString(ErrorLogNumber e) {
	switch (e) {
	case ErrorLogNumber::ERROR_1:
		return "Image sizes are not equal";
	case ErrorLogNumber::ERROR_2:
		return "Image channels are not equal";
	case ErrorLogNumber::WARNING_1:
		return "Imges type are not equal";
	default:
		return "Unimplemented error";
	}
}