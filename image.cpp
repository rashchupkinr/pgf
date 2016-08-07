#include <math.h>
#include "image.h"
#include "polygon.h"
#include "logging.h"

#define EDGE_DIFF 20
vector<edge> Image::find_edges()
{
	vector<edge> edges;
	Image block(3, 3);
	for (int i=0; i<width; i++)
		for (int j=0; j<height; j++) {
			getBlock(i, j, &block);

			int min_min=255, max_max=0;
			for (int ii=0; ii<3; ii++)
				for (int jj=0; jj<3; jj++) {
					int v = block.data[ii+jj*3];
					if (v < min_min)
						min_min = v;
					if (v > max_max)
						max_max = v;
				}
			if (min_min + EDGE_DIFF > max_max)
				continue;
			int val = data[i+j*width];
			if (val < min_min + EDGE_DIFF/2)
				continue;
			if (val > max_max - EDGE_DIFF/2)
				continue;
			int nmin = 0, nmax = 0;
			int min_max = 0, max_min = 255;
			for (int ii=0; ii<3; ii++)
				for (int jj=0; jj<3; jj++) {
					int v = block.data[ii+jj*3];
					if (v < min_min+EDGE_DIFF/2) {
						nmin++;
						if (v > min_max)
							min_max = v;
					}
					if (v > max_max-EDGE_DIFF/2) {
						nmax++;
						if (v < max_min)
							max_min = v;
					}
				}
			if ((nmin >= 2 && nmax >= 2) ||
					(nmin == 1 && nmax >= 3) ||
					(nmax == 1 && nmin >= 3)) {
					edge e;
					e.x = i;
					e.y = j;
					e.min_min = min_min;
					e.min_max = min_max;
					e.max_max = max_max;
					e.max_min = max_min;
					edges.push_back(e);
			}

		}
	return edges;
}

void Image::getBlock(int x, int y, Image *block)
{
	for (int i=-block->width/2; i<1+block->width/2; i++)
		for (int j=-block->height/2; j<1+block->height/2; j++) {
			int ix=i, jy=j;
			if (x+ix < 0)
				ix=-x;
			if (y+jy < 0)
				jy=-y;
			if (x+ix >= width)
				ix=width-x-1;
			if (y+jy >= height)
				jy=height-y-1;
			block->data[i+block->width/2 + block->width*(j+block->height/2)] = data[x+ix + (y+jy)*width];
		}
}

int Image::get(Point p)
{
	int x = round((2*p.x-1)/2);
	int y = round((2*p.y-1)/2);
	if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
		return -1;
	return get(x, y);
}

int Image::get(int x, int y)
{
	if (x<0 || x>=width || y<0 || y>=height)
		return -1;
	return data[x + y*width];
}

void Image::set(Point p, int v)
{
	int x = round((2*p.x-1)/2);
	int y = round((2*p.y-1)/2);
	set(x, y, v);
}

void Image::set(int x, int y, int v)
{
	if (x<0 || x>=width || y<0 || y>=height)
		return;
	data[x + y*width] = v;
}
