#ifndef IMAGE_H
#define IMAGE_H

#include <string.h>
#include <vector>
#include "polmap.h"
#include "pdistribn.h"
using namespace std;

#define	EDGE	-1
struct edge {
	int x;
	int y;
	int max_max, max_min, min_min, min_max;
};

class Image {
	friend class YUVImage;
public:
	int getMaxValue()	{	return 255;		}
	int getWidth()		{	return width;	}
	int getHeight()		{	return height;	}
	// size%2==1
	int get(Point p);
	int get(int x, int y);
	void set(Point p, int v);
	void set(int x, int y, int v);
//	PolMap *getPM()		{	return polmap;	}
	void getBlock(int x, int y, Image *block);
	vector<edge> find_edges();
	Image():/* polmap(0),*/ width(0), height(0), data(0)		{					}
	Image(int _width, int _height, bool gen_polmap = false)
	:// polmap(0),
	  width(_width),
	  height(_height)
	{
		data = new int[width*height];
		memset(data, 0, sizeof(*data)*width*height);
//		if (gen_polmap)
//			polmap = new PolMap(this);
        PDs = new PDistrib *[width*height];
	}
	~Image()
	{
		delete []data;
	}
    void setPD(int x, int y, PDistrib *pd)
	{
        PDs[y*width+x] = pd;
	}
    PDistrib *getPD(int x, int y)
	{
        return PDs[y*width+x];
	}
private:
	int width, height;
	int *data;
//	PolMap *polmap;
    PDistrib **PDs;
};

#endif
