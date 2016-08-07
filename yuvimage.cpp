#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "yuvimage.h"
#include "logging.h"



int YUVImage::getPoint(int plane, int x, int y)
{
	if (width > getWidth(plane) || height > getHeight(plane))
		return -1;
	switch (plane) {
	case PLANE_LUM:
		return lum->data[y*getWidth(plane)+x];
	case PLANE_CR:
		return Cr->data[y*getWidth(plane)+x];
	case PLANE_CB:
		return Cb->data[y*getWidth(plane)+x];
	}
	return -1;
}

int YUVImage::putPoint(int plane, int x, int y, int value)
{
	if (width > getWidth(plane) || height > getHeight(plane))
		return -1;
	switch (plane) {
	case PLANE_LUM:
		lum->data[y*getWidth(plane)+x] = value;
		break;
	case PLANE_CR:
		Cr->data[y*getWidth(plane)+x] = value;
		break;
	case PLANE_CB:
		Cb->data[y*getWidth(plane)+x] = value;
		break;
	}
	return 0;
}

Image *YUVImage::getPlane(int plane)
{
	switch (plane) {
	case PLANE_LUM:
		return lum;
	case PLANE_CR:
		return Cr;
	case PLANE_CB:
		return Cb;
	}
	return 0;
}

size_t YUVImage::getPlaneNum()
{
	return 3;
}

size_t YUVImage::getWidth(int plane)
{
	switch (plane) {
	case PLANE_LUM: {
		return width;
	}
	case PLANE_CR:
	case PLANE_CB: {
		switch (yuvformat) {
		case YUV411:
			return width/2;
		case YUV422:
			return width/2;
		case YUV444:
			return width;
		}
	}
	}
	return 0;
}

size_t YUVImage::getHeight(int plane)
{
	switch (plane) {
	case PLANE_LUM: {
		return height;
	}
	case PLANE_CR:
	case PLANE_CB: {
		switch (yuvformat) {
		case YUV411:
			return height/2;
		case YUV422:
			return height;
		case YUV444:
			return height;
		}
	}
	}
	return 0;
}

int YUVImage::read(FILE *f)
{
	if (width == 0)
		return -1;
	if (!f)
		return -1;
	int nframe = 0;
	size_t offset = nframe*(getWidth(PLANE_LUM)*getHeight(PLANE_LUM)+getWidth(PLANE_CR)*getHeight(PLANE_CR)+getWidth(PLANE_CB)*getHeight(PLANE_CB));
	if (fseek(f, offset, SEEK_SET))
		return -1;
	if (fread(clum, sizeof(char), getWidth(PLANE_LUM)*getHeight(PLANE_LUM), f) !=
			getWidth(PLANE_LUM)*getHeight(PLANE_LUM))
		return -1;
	if (fread(cCr, sizeof(char), getWidth(PLANE_CR)*getHeight(PLANE_CR), f) !=
			getWidth(PLANE_CR)*getHeight(PLANE_CR))
		return -1;
	if (fread(cCb, sizeof(char), getWidth(PLANE_CB)*getHeight(PLANE_CB), f) !=
			getWidth(PLANE_CB)*getHeight(PLANE_CB))
		return -1;

	for (int i=0; i<getWidth(PLANE_LUM)*getHeight(PLANE_LUM); i++)
		lum->data[i] = clum[i];
	for (int i=0; i<getWidth(PLANE_CR)*getHeight(PLANE_CR); i++)
		Cr->data[i] = cCr[i];
	for (int i=0; i<getWidth(PLANE_CB)*getHeight(PLANE_CB); i++)
		Cb->data[i] = cCb[i];
	return 0;
}

int YUVImage::write(FILE *f)
{
	if (width == 0 || height == 0)
		return -1;
	if (!f)
		return -1;
	int nframe = 0;
	for (int i=0; i<getWidth(PLANE_LUM)*getHeight(PLANE_LUM); i++)
		clum[i] = lum->data[i];
	for (int i=0; i<getWidth(PLANE_CR)*getHeight(PLANE_CR); i++)
		cCr[i] = Cr->data[i];
	for (int i=0; i<getWidth(PLANE_CB)*getHeight(PLANE_CB); i++)
		cCb[i] = Cb->data[i];

	size_t offset = nframe*(getWidth(PLANE_LUM)*getHeight(PLANE_LUM)+getWidth(PLANE_CR)*getHeight(PLANE_CR)+getWidth(PLANE_CB)*getHeight(PLANE_CB));
	if (fseek(f, offset, SEEK_SET))
		return -1;
	if (fwrite(clum, sizeof(char), getWidth(PLANE_LUM)*getHeight(PLANE_LUM), f) !=
			getWidth(PLANE_LUM)*getHeight(PLANE_LUM))
		return -1;
	if (fwrite(cCr, sizeof(char), getWidth(PLANE_CR)*getHeight(PLANE_CR), f) !=
			getWidth(PLANE_CR)*getHeight(PLANE_CR))
		return -1;
	if (fwrite(cCb, sizeof(char), getWidth(PLANE_CB)*getHeight(PLANE_CB), f) !=
			getWidth(PLANE_CB)*getHeight(PLANE_CB))
		return -1;
	return 0;
}

YUVImage::YUVImage(YUV_FORMAT _yuvformat, size_t _width, size_t _height)
: yuvformat(_yuvformat),
  width(_width),
  height(_height)
{
	lum = new Image(getWidth(PLANE_LUM), getHeight(PLANE_LUM));
	Cr = new Image(getWidth(PLANE_CR), getHeight(PLANE_CR));
	Cb = new Image(getWidth(PLANE_CB), getHeight(PLANE_CB));
	clum = new unsigned char[getWidth(PLANE_LUM) * getHeight(PLANE_LUM)];
	cCr = new unsigned char[getWidth(PLANE_CR) * getHeight(PLANE_CR)];
	cCb = new unsigned char[getWidth(PLANE_CB) * getHeight(PLANE_CB)];
}

YUVImage::YUVImage(YUV_FORMAT _yuvformat, CIF_FORMAT _cifformat)
{
	yuvformat = _yuvformat;
	cifformat = _cifformat;
	CIF_FORMAT_SIZE *fmt = CIF_FORMAT_SIZES;
	while (fmt->format != SF_NONE) {
		if (fmt->format == cifformat) {
			YUVImage(yuvformat, fmt->width, fmt->height);
			break;
		}
		fmt++;
	}
	if (fmt->format == SF_NONE)
		throw std::runtime_error("CIF format not found.");

}

YUVImage::YUVImage(YUVImage *base)
:YUVImage(base->getYUVFormat(), base->getWidth(0), base->getHeight(0))
{
}

int YUVImage::copy(YUVImage *base)
{
	if (getPlaneNum() != base->getPlaneNum())
		return -1;
	for (int k=0; k<getPlaneNum(); k++)
		memcpy(getPlane(k)->data, base->getPlane(k)->data, sizeof(*(getPlane(k)->data)) * getWidth(k) * getHeight(k));
	return 0;
}

YUVImage::~YUVImage()
{
	delete lum;
	delete Cr;
	delete Cb;
	delete []clum;
	delete []cCr;
	delete []cCb;
}
