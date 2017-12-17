#include "pequal.h"

#define PEQUAL_PROB	1

int PEqual::predict(int x, int y, PDistrib *pd, double priority, int rad)
{
	Image *img = yuvimage->getPlane(plane);
    if (!img || x<0 || x>=img->getWidth() || y<0 || y>=img->getHeight())
        return -1;
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
        return -1;

    val = img->getPointDir(x, y, dir);
	if (val < 0)
        return -1;
    int radius = rad;
	if (radius > val)
		radius = val;
	if (radius > img->getMaxValue() - val)
		radius = img->getMaxValue() - val;
    pd->addSpikeEllipse(val, radius, prob*priority);
    return val;
}
