#include "pequal.h"

#define PEQUAL_PROB	0.5

PDistrib PEqual::predict(int x, int y)
{
	if (!img || x<0 || x>=img->getWidth() || y<0 || y>=img->getHeight())
		return PDistrib::getNullPD();
	PDistrib pd = PDistrib::getNullPD();
	int val=-1;
	float prob;
	if (dir == PREDICTOR_DIR_L) {
		if (x > 0) {
			val = img->get(x-1, y);
			prob = PEQUAL_PROB;
//			pd.addSpikeEllipse(img->get(x-1, y), getPredParam().SpikeRadius, PEQUAL_PROB);
		}
	} else if (dir == PREDICTOR_DIR_LU) {
		if (x > 0 && y > 0) {
			val = img->get(x-1, y-1);
			prob = PEQUAL_PROB/1.5;
//			pd.addSpikeEllipse(img->get(x-1, y-1), getPredParam().SpikeRadius, PEQUAL_PROB/1.5);
		}
	} else if (dir == PREDICTOR_DIR_U) {
		if (y > 0) {
			val = img->get(x, y-1);
			prob = PEQUAL_PROB;
//			pd.addSpikeEllipse(img->get(x, y-1), getPredParam().SpikeRadius, PEQUAL_PROB);
		}
	} else if (dir == PREDICTOR_DIR_RU) {
		if (x < img->getWidth()-1 && y > 0) {
			val = img->get(x+1, y-1);
			prob = PEQUAL_PROB/1.5;
//			pd.addSpikeEllipse(img->get(x+1, y-1), getPredParam().SpikeRadius, PEQUAL_PROB/1.5);
		}
	} else
		return pd;
	if (val < 0)
		return pd;
	int radius = getPredParam().SpikeRadius;
	if (radius > val)
		radius = val;
	if (radius > img->getMaxValue() - val)
		radius = img->getMaxValue() - val;
	pd.addSpikeEllipse(val, radius, prob);
	return pd;
}
