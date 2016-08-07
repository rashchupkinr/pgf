/*
 * polmap.h
 *
 *  Created on: 30.06.2015
 *      Author: roman
 */

#ifndef POLMAP_H_
#define POLMAP_H_

#include <vector>
#include <string>
using namespace std;

class Image;
class Point;
class Polygon;
class PolMap {
public:
	PolMap(Image *_img);
	virtual ~PolMap();
	void clear();
	int addPolygon(Polygon pg);
	vector<Polygon> getPolygons(Point p);
	Polygon getPolygon(Point p);
	string print();
	void dlog(int level);
private:
	vector<int> *getPgIdx(Point p);
	Image *img;
	vector<Polygon> polygons;
	vector<int> *polmap;
};

#endif /* POLMAP_H_ */
