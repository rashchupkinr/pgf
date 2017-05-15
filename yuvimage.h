#ifndef _YUVIMAGE_H
#define _YUVIMAGE_H
#include <stdio.h>
#include "genimage.h"
#include "image.h"
#include "c3x.h"

enum YUV_FORMAT {	YUV411, YUV422, YUV444	};

#define PLANE_LUM	0
#define PLANE_CR	1
#define PLANE_CB	2

class YUVImage : public GenImage {
public:
	int getPoint(int plane, int x, int y);
	int putPoint(int plane, int x, int y, int value);
	Image *getPlane(int plane);
	size_t getPlaneNum();
	size_t getWidth(int plane);
	size_t getHeight(int plane);
	c3x get(int x, int y)	{	return c[y*width + x];	}
	void set(int x, int y, c3x _c) {	c[y*width+x] = _c;	}

	int read(FILE *f);
	int write(FILE *f);
	int writeRow(FILE *f, int pnum, int row);
	YUV_FORMAT getYUVFormat()		{	return yuvformat;	};
	int getMaxValue()	{	return 0xffffff;		}
	YUVImage()	{	width = height = 0;	}
	YUVImage(YUV_FORMAT _yuvformat, CIF_FORMAT _cifformat);
	YUVImage(YUV_FORMAT _yuvformat, size_t _width, size_t _height);
	YUVImage(YUVImage *base);
	int copy(YUVImage *base);
	~YUVImage();
	void setPD(int x, int y, PDistrib &pd)
	{
		PDs[y*width+x] = pd;
	}
	PDistrib getPD(int x, int y)
	{
		return PDs[y*width+x];
	}
private:
	YUV_FORMAT yuvformat;
	size_t width, height;
	Image *lum, *Cr, *Cb;
	unsigned char *clum, *cCr, *cCb, *crow;
	c3x *c;
	PDistrib *PDs;
};

#endif
