#include "plinear.h"

PDistrib PLinear::predict(int x, int y)
{
	if (!img || x<0 || x>=img->getWidth() || y<0 || y>=img->getHeight())
		return PDistrib::getNullPD();
	PDistrib pd = PDistrib::getNullPD();
	int v0 = 0, v1 = 0;
	long v;
	float prob;
	if (dir == PREDICTOR_DIR_L)
		if (x > 1) {
			long delta = img->get(x-1, y)-img->get(x-2, y);
			if (abs(delta) > getPredParam().SpikeRadius*2)
				return pd;
			v = img->get(x-1, y)+delta;
			prob = 0.15;
//			pd.addSpikeEllipse(v, getPredParam().SpikeRadius, 0.15);
		}
	if (dir == PREDICTOR_DIR_LU)
		if (x > 1 && y > 1) {
			long delta = img->get(x-1, y-1)-img->get(x-2, y-2);
			if (abs(delta) > getPredParam().SpikeRadius*2)
				return pd;
			v = img->get(x-1, y-1)+delta;
			prob = 0.1;
//			pd.addSpikeEllipse(v, getPredParam().SpikeRadius, 0.1);
		}
	if (dir == PREDICTOR_DIR_U)
		if (y > 1) {
			long delta = img->get(x, y-1)-img->get(x, y-2);
			if (abs(delta) > getPredParam().SpikeRadius*2)
				return pd;
			v = img->get(x, y-1)+delta;
			prob = 0.15;
//			pd.addSpikeEllipse(v, getPredParam().SpikeRadius, 0.15);
		}
	if (dir == PREDICTOR_DIR_RU)
		if (x < img->getWidth()-2 && y > 1) {
			long delta = img->get(x+1, y-1)-img->get(x+2, y-2);
			if (abs(delta) > getPredParam().SpikeRadius*2)
				return pd;
			v = img->get(x+1, y-1)+delta;
			prob = 0.1;
//			pd.addSpikeEllipse(v, getPredParam().SpikeRadius, 0.1);
		}
	int radius = getPredParam().SpikeRadius;
	if (radius > v)
		radius = v;
	if (radius > img->getMaxValue() - v)
		radius = img->getMaxValue() - v;
	pd.addSpikeEllipse(v, radius, prob);
	return pd;
}
