#include "ModuleTexture.h"

#include "GL/glew.h"

#include "Utils/Logging.h"

typedef unsigned uint;

uint ModuleTexture::CreateTexture(cv::Mat* resource) {
	GLenum format = GetTextureFormat(resource->channels());
	GLenum type = GetTextureGLType(resource->type());

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
	GLenum type = GetTextureGLType(resource->type());

	if ((format || type) == 0) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, resource->cols, resource->rows, 0, format, type, resource->data);
}

void ModuleTexture::GenerateDiffImage(cv::Mat* resOriginal, cv::Mat* resCompare, cv::Mat* resOutput, CompareType compareType, float threshold) {
	cv::absdiff(*resOriginal, *resCompare, *resOutput);

	if (compareType == CompareType::BITMASK) {
		for (int j = 0; j < resOutput->rows; ++j) {
			for (int i = 0; i < resOutput->cols; ++i) {
				cv::Vec3b pix = resOutput->at<cv::Vec3b>(j, i);
				int val = (pix[0] + pix[1] + pix[2]);
				if (val > threshold) {
					resOutput->at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
				}
			}
		}
	}
}

double ModuleTexture::CalculatePSN(cv::Mat* resOriginal, cv::Mat* resCompare) {
	cv::Mat s1;
	absdiff(*resOriginal, *resCompare, s1);		 // |I1 - I2|
	s1.convertTo(s1, CV_32F);					 // cannot make a square on 8 bits
	s1 = s1.mul(s1);							 // |I1 - I2|^2
	cv::Scalar s = sum(s1);						 // sum elements per channel
	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
	if (sse <= 1e-10)							 // for small values return zero
		return 0;
	else {
		double mse = sse / (double) (resOriginal->channels() * resOriginal->total());
		double psnr = 10.0 * log10((255 * 255) / mse);
		return psnr;
	}
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

uint ModuleTexture::GetTextureGLType(int type) {
	uchar depth = type & CV_MAT_DEPTH_MASK;

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

uchar ModuleTexture::GetTextureType(int type) {
	return type & CV_MAT_DEPTH_MASK;
}

std::string ModuleTexture::GetTextureTypeToString(int type) {
	uchar depth = type & CV_MAT_DEPTH_MASK;

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

void ModuleTexture::DeleteTexture(uint& glTexture) {
	if (glTexture) {
		glDeleteTextures(1, &glTexture);
		glTexture = 0;
	}
}
