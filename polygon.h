#ifndef PGON_H_
#define PGON_H_
#include <vector>
#include <cstdint>
#include "image.h"
#include "bitarr.h"

using namespace std;

/*
#define EPS 1E-15
#define DLIMIT 1E-4
#define DEDGE_VERT 1E-5
#define DEDGE_HOR 1E-13				//  DEDGE_HOR < DEDGE_VERT*DLIMIT/HEIGHT
*/
#undef FLOAT128

#ifdef FLOAT128
typedef __float128 Cfloat;
#else
typedef double Cfloat;		// MAX_DIM=7
#endif
#define MAX_DIM_B 7
#define MAX_DIM	(1UL<<MAX_DIM_B)
#define DLIMIT ((Cfloat)1.0/MAX_DIM/4)
#define DEDGE_VERT (DLIMIT/MAX_DIM/4)
#define DEDGE_HOR (DEDGE_VERT/MAX_DIM*DLIMIT/4)
#define EPS (DEDGE_HOR/MAX_DIM/4)
#define FIS_EQ(a, b) ((a) >= (b)-EPS && (a) <= (b)+EPS)
#define MARGIN_DIFF	1.0
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (a) : (b))

class Point {
public:
	Point(): x(0), y(0) {}
	Point(Cfloat _x, Cfloat _y) : x(_x), y(_y)	{}
	Cfloat x, y;
	Cfloat Distance(Point p1);
	bool Between(Point p0, Point p1);
	Point ClosesetOnLine(Point p0, Point p1);
	bool operator==(const Point &p) const;
	string print();
	void dlog(int level);
};

class Polygon {
public:
	Polygon(Image *_img): img(_img) {}
	bitarr getID()						{	return id;						}
	int setVertex(vector<Point> _vertex);
	Point getVertex(int n);
	int getVertexNum()					{	return vertex.size();			}
	int getBase();
	int getMarginLen();
	Point getMarginPoint(int m);
	int getMarginNum(Point p);
	int getFullMarginLen();
	Point getFullMarginPoint(int m);
	int getFullMarginNum(Point p);
	int getArea();
	int Split(Point p1, Point p2, Polygon &pg1, Polygon &pg2);
	Point shiftedPoint(Point p, Point v0, Point v1, int sign_x, int sign_y);
	Point shiftPoint(Point p, int e);
	int sameEdge(Point p1, Point p2);
	Cfloat getPointY(Point p, int edge);
	void Normalize();
	// point belongs to polygon if area down to the point belongs to polygon, at [x, height] if right area belongs, at [width, height] if higher area belongs
	bool isInside(Point p);
	bool isInside(Cfloat x, Cfloat y)	{	return isInside(Point(x, y));	}
	int getVertex(Point p);
	Point getDim0()						{	return dim0;					}
	Point getDim1()						{	return dim1;					}
	vector<int> findEdges(Point p);
	int findEdge(Point p);
	bool isMargin(Point p);
	string print();
	void dlog(int level);
private:
	int setVertexExact(vector<Point> _vertex);
	void computeDimensions();
	void computeMargin();
	void computeFullMargin();
	bool MarginIsInside(Point p, bool horiz=false);
	Image *img;
	vector<Point> vertex;
	Point dim0, dim1;
	// priority: left then high belongs to polygon
	vector<Point> margin;
	vector<Point> full_margin;
	vector<Point> pinside;		// vertex belonging to polygon after some split
	vector<Point> poutside;		// vertex belonging to polygon after some split
	bitarr id;
};

#endif
