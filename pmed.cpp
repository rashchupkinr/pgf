#include <math.h>
#include "pmed.h"
#include "logging.h"
#define PMED_PROB 2

int PMed::predict(int x, int y, PDistrib *pd, double priority, int rad)
{
	Image *img = yuvimage->getPlane(plane);
    if (!img || x<0 || x>=img->getWidth() || y<0 || y>=img->getHeight())
        return -1;
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
    if (n < 0.1)
        return -1;
	val /= n;
    int radius = rad;
/*    if (max >= min)
		radius += (max - min)/2;
*/    if (2*radius > val)
        radius = val/2;
    if (2*radius > img->getMaxValue() - val)
        radius = (img->getMaxValue() - val)/2;
//	if (n>0)
    pd->addSpikeEllipse(val, 2*radius, PMED_PROB*priority);
    return val;
}
