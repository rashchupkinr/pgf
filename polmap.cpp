/*
 * polmap.cpp
 *
 *  Created on: 30.06.2015
 *      Author: roman
 */

#include <math.h>
#include "polmap.h"
#include "image.h"
#include "polygon.h"
#include "logging.h"

void PolMap::clear()
{
	polygons.clear();
	for (int i=0; i<img->getWidth(); i++)
		for (int j=0; j<img->getHeight(); j++) {
			vector<int> *pgidx = getPgIdx(Point(i+0.5, j+0.5));
			if (!pgidx)
				continue;
			pgidx->clear();
		}
}

int PolMap::addPolygon(Polygon pg)
{
	bool error = false;
	polygons.push_back(pg);
	int idx = polygons.size() - 1;
	for (int x=floor(pg.getDim0().x); x<=ceil(pg.getDim1().x); x++)
		for (int y=floor(pg.getDim0().y); y<=ceil(pg.getDim1().y); y++) {
			Point p = Point(x+0.5, y+0.5);
			if (pg.isInside(p)) {
				vector<int> *pgidx = getPgIdx(p);
				if (!pgidx)
					return -1;
				if (pgidx->size() > 1) {
					int old_idx = (*pgidx)[pgidx->size() - 1];
					if (old_idx != -1) {
						Polygon pg0 = polygons[old_idx];
						bool error_old = false;
						if (pg.getID().size() == 0 && pg0.getID().size() == 0) {
							error_old = false;
						} else if (pg0.getID().size() + 1 != pg.getID().size()) {
							error_old = true;
						} else
							for (int i=0; i<pg0.getID().size(); i++)
								if (pg0.getID().get(i) != pg.getID().get(i)) {
									error_old = true;
									break;
								}
						if (error_old) {
							::dlog(LOG_ERROR, "polmap error addPolygon: conflict[%d,%d] old=%s new=%s\n", x, y, pg0.getID().print().c_str(), pg.getID().print().c_str());
							error = true;
							continue;
						}
					}
				}
				pgidx->push_back(idx);
			}
		}
	return -error;
}

vector<Polygon> PolMap::getPolygons(Point p)
{
	vector<Polygon> pgs;
	vector<int> *pgidx = getPgIdx(p);
	if (!pgidx)
		return pgs;
	for (int i=0; pgidx->size(); i++)
		pgs.push_back(polygons[(*pgidx)[i]]);
	return pgs;
}

Polygon PolMap::getPolygon(Point p)
{
	Polygon pg(img);
	vector<int> *pgidx = getPgIdx(p);
	if (!pgidx)
		return pg;
	return polygons[(*pgidx)[pgidx->size() - 1]];
}

vector<int> *PolMap::getPgIdx(Point p)
{
	int x = round((2*p.x-1)/2);
	int y = round((2*p.y-1)/2);
	if (x < 0 || x >= img->getWidth() || y < 0 || y >= img->getHeight())
		return 0;
	return &polmap[x + y*img->getWidth()];
}

string PolMap::print()
{
	string str = "polmap n=";
	str += std::to_string(polygons.size());
	str += "\n";
	for (int i=0; i<polygons.size(); i++)
		str += polygons[i].print();
	return str;
}

void PolMap::dlog(int level)
{

}

PolMap::PolMap(Image *_img) {
	// TODO Auto-generated constructor stub
	img = _img;
	polmap = new vector<int>[img->getWidth() * img->getHeight()];
	clear();
}

PolMap::~PolMap() {
	if (polmap)
		delete []polmap;
	// TODO Auto-generated destructor stub
}

