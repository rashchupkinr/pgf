#include "predictor.h"
#include "image.h"

PredParam::PredParam()
: SpikeRadius(0)
{
}

Predictor::Predictor(YUVImage *_img, int _dir, int _plane)
: yuvimage(_img),
  dir(_dir),
  plane(_plane)
{
}

int Predictor::predict(int x, int y, PDistrib *pd, double priority, int rad)
{
	Image *img = yuvimage->getPlane(plane);
    pd->setNullPD();
	if (!img || x < 0 || x >= img->getWidth() || y < 0 || y >= img->getHeight())
        return -1;
    pd->setUniformPD(img->getMaxValue());
    return -1;
}

PredParam Predictor::getPredParam()
{
	PredParam param;
    param.SpikeRadius = 4;
	if (!yuvimage)
		return param;
    param.SpikeRadius = 4;//(img->getMaxValue()+1)/64;
    if (plane == PLANE_LUM)
        param.SpikeRadius = 7;
    else
        param.SpikeRadius = 3;
    return param;
}
