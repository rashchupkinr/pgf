#include <math.h>
#include "pmed.h"
#include "logging.h"
#define PMED_PROB 0.3

PDistrib PMed::predict(int x, int y)
{
	Image *img = yuvimage->getPlane(plane);
	if (!img || x<0 || x>=img->getWidth() || y<0 || y>=img->getHeight())
		return PDistrib::getNullPD();
	PDistrib pd = PDistrib::getNullPD();
	int v;
	float val = 0;
	float n = 0;
	int min = 255, max = 0;
	if (x > 0) {
		v = img->get(x-1, y);
		val += v;
		n++;
		if (v < min)
			min = v;
		if (v > max)
			max = v;
	}
	if (x > 0 && y > 0) {
		v = img->get(x-1, y-1);
		val += v/sqrt(2);
		n += 1/sqrt(2);
		if (v < min)
			min = v;
		if (v > max)
			max = v;
	}
	if (y > 0) {
		v = img->get(x, y-1);
		val += v;
		n++;
		if (v < min)
			min = v;
		if (v > max)
			max = v;
	}
	if (x < img->getWidth()-1 && y > 0) {
		v = img->get(x+1, y-1);
		n += 1/sqrt(2);
		val += v/sqrt(2);
		if (v < min)
			min = v;
		if (v > max)
			max = v;
	}
	if (!n)
		return pd;
	val /= n;
	int radius = getPredParam().SpikeRadius;
	if (max >= min)
		radius += (max - min)/2;
	if (radius > val)
		radius = val;
	if (radius > img->getMaxValue() - val)
		radius = img->getMaxValue() - val;

//	if (n>0)
		pd.addSpikeEllipse(val, radius, PMED_PROB);
	return pd;
}
