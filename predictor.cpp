#include "predictor.h"

PredParam::PredParam()
: SpikeRadius(0)
{
}

Predictor::Predictor(Image *_img, int _dir)
: img(_img),
  dir(_dir)
{
}

PDistrib Predictor::predict(int x, int y)
{
	if (!img || x < 0 || x >= img->getWidth() || y < 0 || y >= img->getHeight())
		return PDistrib::getNullPD();
	return PDistrib::getUniformPD(img->getMaxValue());
}

PredParam Predictor::getPredParam()
{
	PredParam param;
	param.SpikeRadius = 4;
	if (!img)
		return param;
	param.SpikeRadius = 4;//(img->getMaxValue()+1)/64;
	return param;
}
