#ifndef _GENIMAGE_H
#define _GENIMAGE_H
#include <stddef.h>
#include "image.h"

enum CIF_FORMAT {
	SF_NONE, SF_SQCIF, SF_QCIF, SF_CIF, SF_4CIF, SF_16CIF
};
struct CIF_FORMAT_SIZE {
	CIF_FORMAT format;
	int width;
	int height;
};

class GenImage {
public:
	CIF_FORMAT getCIFFormat()			{	return cifformat;	}
	virtual size_t getPlaneNum() = 0;
	virtual Image *getPlane(int plane) = 0;
	virtual size_t getWidth(int plane) = 0;
	virtual size_t getHeight(int plane) = 0;
	virtual int getPoint(int plane, int x, int y) = 0;
	virtual int putPoint(int plane, int x, int y, int value) = 0;
	double diff(GenImage *img);
protected:
	static struct CIF_FORMAT_SIZE CIF_FORMAT_SIZES[];
	CIF_FORMAT cifformat;
};
#endif

