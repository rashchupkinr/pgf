#include <math.h>
#include "psim.h"
#include "logging.h"

#define PSIM_DIFF		3
#define PSIM_DISTANCE	100
#define	PSIM_HITS		50
#define PSIM_PROB		1.5

int PSim::predict(int x, int y, PDistrib *pd, double priority, int rad)
{
    Image *img = yuvimage->getPlane(plane);
	if (!img || x-1<0 || x+1>=img->getWidth() || y-1<0 || y>=img->getHeight())
        return -1;
    int Diff = getPredParam().SpikeRadius;
	int nhits = 0;
	float p;
	int prec1, prec2, prec3;
	float prec;
	for (int xx=x+PSIM_DISTANCE/2; xx>x-PSIM_DISTANCE/2 && xx>0 && nhits<PSIM_HITS; xx--)
		for (int yy=y; yy>y-PSIM_DISTANCE && yy>0 && nhits<PSIM_HITS; yy--) {
			if (xx >= x && y == yy)
				continue;
			if (xx-2<0 || yy-2<0 || xx+1>=img->getWidth())
				continue;

			if (img->get(xx-2, yy-2) == img->get(xx-1, yy-1))
				p = 0.5;
			else
				p = (img->get(x-1, y-1)-img->get(xx-1,yy-1))/(img->get(xx-2, yy-2)-img->get(xx-1, yy-1));
			if (p < 0 || p > 1)
				continue;

			int v1 = img->get(xx-1, yy) + (img->get(xx-2, yy-1)-img->get(xx-1, yy))*p;
			if (v1 < img->get(x-1, y) - PSIM_DIFF || v1 > img->get(x-1, y) + PSIM_DIFF)
				continue;
			prec1 = abs(v1 - img->get(x-1, y));

			int v2 = img->get(xx, yy-1) + (img->get(xx, yy-2)-img->get(xx, yy-1))*p;
			if (v2 < img->get(x, y-1) - PSIM_DIFF || v2 > img->get(x, y-1) + PSIM_DIFF)
				continue;
			prec2 = abs(v2 - img->get(x, y-1));

			int v3 = img->get(xx+1, yy-1) + (img->get(xx, yy-2)-img->get(xx+1, yy-1))*p;
			if (v3 < img->get(x+1, y-1) - PSIM_DIFF || v3 > img->get(x+1, y-1) + PSIM_DIFF)
				continue;
			prec3 = abs(v3 - img->get(x+1, y-1));

			int v = img->get(xx, yy) + (img->get(xx-1, yy-1)-img->get(xx, yy))*p;
			dlog(LOG_ALL,"hit%d{%d:%d/%d:%d %3f} ", v,y,x,yy,xx,p);
            int radius = rad;
			if (radius > v)
				radius = v;
			if (radius > img->getMaxValue() - v)
				radius = img->getMaxValue() - v;
			int distance = sqrt((x-xx)*(x-xx)+(y-yy)*(y-yy));
            pd->addSpikeEllipse(v, radius, PSIM_PROB*(1-(prec1+prec2+prec3)/(PSIM_DIFF*3))*priority);
			nhits++;
		}
	if (nhits>0)
		dlog(LOG_ALL,"\n");
    return -1;
}
