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

void Predictor::predict(int x, int y, PDistrib *pd)
{
	Image *img = yuvimage->getPlane(plane);
    pd->setNullPD();
	if (!img || x < 0 || x >= img->getWidth() || y < 0 || y >= img->getHeight())
        return;
    pd->setUniformPD(img->getMaxValue());
}

PredParam Predictor::getPredParam()
{
	PredParam param;
	param.SpikeRadius = 4;
	if (!yuvimage)
		return param;
	param.SpikeRadius = 4;//(img->getMaxValue()+1)/64;
	return param;
}
