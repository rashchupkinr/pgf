#ifdef __FLOAT128
extern "C" {
#include <quadmath.h>
}
#endif
#include <math.h>
#include "polygon.h"
#include "logging.h"

#define POLYGON_TRACE

int Polygon::setVertex(vector<Point> _vertex)
{
	vertex = _vertex;
	for (int i=0; i<vertex.size(); i++) {
		if (vertex[i].y >  img->getHeight()-0.5-EPS)
			vertex[i].y += DLIMIT;
		if (vertex[i].x > img->getWidth()-0.5-EPS)
			vertex[i].x += DLIMIT;
	}
	computeDimensions();
	computeMargin();
	computeFullMargin();
	return 0;
}

int Polygon::setVertexExact(vector<Point> _vertex)
{
	vertex = _vertex;
	computeDimensions();
	computeMargin();
	computeFullMargin();
	return 0;
}

Point Polygon::getVertex(int n)
{
	if (n >= vertex.size() || n < 0)
		return Point(-1, -1);
	return vertex[n];
}

int Polygon::getBase()
{
	if (vertex.size() == 0)
		return -1;
	int x=vertex[0].x, y=vertex[0].y;
	for (int i=1; i<vertex.size(); i++)
		if (vertex[i].x < x)
			x = vertex[i].y;
	int n = 0;
	for (int i=1; i<vertex.size(); i++)
		if (vertex[i].x == x)
			if (vertex[i].y < y) {
				y = vertex[i].y;
				n = i;
			}
	return n;
}

void Polygon::computeDimensions()
{
	dim0 = Point(img->getWidth(), img->getHeight());
	dim1 = Point(0, 0);
	for (int i=0; i<vertex.size(); i++) {
		if (vertex[i].x < dim0.x)
			dim0.x = vertex[i].x;
		if (vertex[i].y < dim0.y)
			dim0.y = vertex[i].y;
		if (vertex[i].x > dim1.x)
			dim1.x = vertex[i].x;
		if (vertex[i].y > dim1.y)
			dim1.y = vertex[i].y;
	}
}

void Polygon::computeMargin()
{
	margin.clear();
	for (Cfloat x=dim0.x; x<=dim1.x; x++)
		for (Cfloat y=dim0.y; y<=dim1.y; y++) {
			Point p = Point(x, y);
			if (isMargin(p)) {
				margin.push_back(p);
			}
		}
}

void Polygon::computeFullMargin()
{
	full_margin.clear();
	full_margin.insert(full_margin.begin(), vertex.begin(), vertex.end());
	for (Cfloat x=dim0.x; x<=dim1.x; x++)
		for (Cfloat y=dim0.y; y<=dim1.y; y++) {
			Point p = Point(x, y);
			if (getVertex(p) >= 0)
				continue;
			if (isMargin(p)) {
				full_margin.push_back(p);
			}
		}
}

Point Polygon::shiftedPoint(Point p, Point v0, Point v1, int sign_x, int sign_y)
{
/*
	if (v0.x != v1.x) {
		Cfloat x = p.x + (v1.x - v0.x)/1000;
		Cfloat y = v0.y + (v1.y - v0.y)/(v1.x - v0.x)*(p.x - v0.x);
		return point (x, y)
	} else if (v1.y != v0.y) {
		Cfloat y = p.y + (v1.y - v0.y)/1000;
		Cfloat x = v0.x + (v1.x - v0.x)/(v1.y - v0.y)*(p.y - v0.y);
		return point(x, y);
	}
	*/
	return Point(0, 0);
}

Point Polygon::shiftPoint(Point p, int e)
{
	Point v0 = vertex[e];
	Point v1 = vertex[(e + 1) % vertex.size()];
	Point pp = p.ClosesetOnLine(v0, v1);
	Cfloat diff = MARGIN_DIFF - 2*DLIMIT*MAX(img->getWidth(), img->getHeight()) + EPS;
#ifdef POLYGON_TRACE
::dlog(LOG_NORMAL,"sp %d",e);
p.dlog(LOG_NORMAL);v0.dlog(LOG_NORMAL);v1.dlog(LOG_NORMAL);p.ClosesetOnLine(v0,v1).dlog(LOG_NORMAL);
::dlog(LOG_NORMAL, "dist=%.15f\n", pp.Distance(p));
#endif
	if (pp.Between(v0, v1)) {
		if (pp.Distance(p) < diff) {
			if (pp.Distance(v0) < DLIMIT*sqrt(2)+EPS)
				return v0;
			if (pp.Distance(v1) < DLIMIT*sqrt(2)+EPS)
				return v1;
			return pp;
		} else
			return Point(-1, -1);
	} else {
		Cfloat d0 = p.Distance(v0);
		Cfloat d1 = p.Distance(v1);
		if (d0 < d1) {
			if (d0 < diff)
				return v0;
		} else {
			if (d1 < diff)
				return v1;
		}
		return Point(-1, 1);
	}
	if (!isMargin(p))
		return Point(-1, -1);
/*		return point(x, y);
	} else
		return point(-1, -1);
		*/
}

int Polygon::Split(Point p0, Point p1, Polygon &pg0, Polygon &pg1)
{
	if (getFullMarginNum(p0)<0 || getFullMarginNum(p1)<0)
//	if (!isMargin(p0) || !isMargin(p1))
		return -1;
if (0)
	if (sameEdge(p0, p1)) {
#ifdef POLYGON_TRACE
		::dlog(LOG_NORMAL,"can't split: same edge.\n");
#endif
		return -1;
	}

	vector<int> ve0 = findEdges(p0);
	vector<int> ve1 = findEdges(p1);
	if (!ve0.size() || !ve1.size())
		return -1;
#ifdef POLYGON_TRACE
	p0.dlog(LOG_NORMAL);p1.dlog(LOG_NORMAL);
	::dlog(LOG_NORMAL, "\nve0=");
	for (int i=0; i<ve0.size(); i++)
		::dlog(LOG_NORMAL, "%d,", ve0[i]);
	::dlog(LOG_NORMAL, "\nve1=");
	for (int i=0; i<ve1.size(); i++)
		::dlog(LOG_NORMAL, "%d,", ve1[i]);
	::dlog(LOG_NORMAL, "\n");
#endif

	int e0 = ve0[0];
	if (ve0.size()>1 && ve0[ve0.size()-1]==vertex.size()-1)
		e0 = ve0[ve0.size()-1];
	int e1 = ve1[ve1.size()-1];

//	if (getVertex(p0)<0 && getVertex(p1)<0) {
		e0 = findEdge(p0);
		e1 = findEdge(p1);
//	}
	if (e0 == e1) {
		if (ve0.size() > 1) {
			Cfloat d = MARGIN_DIFF;
			for (int i=0; i<ve0.size(); i++)
				if (e0 != ve0[i]) {
					Point v0 = vertex[ve0[i]];
					Point v1 = vertex[(ve0[i] + 1) % vertex.size()];
					Point cl = p0.ClosesetOnLine(v0, v1);
					if (p0.Distance(cl) < d) {
						d = p0.Distance(cl);
						e0 = ve0[i];
					}
				}
		} else if (ve1.size() > 1) {
			Cfloat d = MARGIN_DIFF;
			for (int i=0; i<ve1.size(); i++) {
				if (e1 != ve1[i]) {
					Point v0 = vertex[ve1[i]];
					Point v1 = vertex[(ve1[i] + 1) % vertex.size()];
					Point cl = p0.ClosesetOnLine(v0, v1);
					if (p1.Distance(cl) < d) {
						d = p1.Distance(cl);
						e1 = ve1[i];
					}
				}
			}
		}
	}
	if (e0 > e1) {
		Point p = p0;
		p0 = p1;
		p1 = p;
		int e = e0;
		e0 = e1;
		e1 = e;
	}

	if (p0 == p1 && ve0.size() >= 2) {
		e0 = ve0[0];
		e1 = ve0[ve0.size()-1];
	}
#ifdef POLYGON_TRACE
	::dlog(LOG_NORMAL,"e0=%d e1=%d\n", e0, e1);
#endif
/*	for (int i=0;i<ve1.size(); i++)
		if (ve1[i] < e1)
			e1 = ve1[i];
	for (int i=0;i<ve2.size(); i++)
		if (ve2[i] > e2)
			e2 = ve2[i];
			*/
    Point pp0 = p0, pp1 = p1;
    if (p0.x >= 0.5 && p0.x <= img->getWidth()-0.5 && FIS_EQ(p0.x - trunc(p0.x), 0.5)) {
    	pp0 = shiftPoint(p0, e0);
    }
    if (p1.x >= 0.5 && p1.x <= img->getWidth()-0.5 && FIS_EQ(p1.x - trunc(p1.x), 0.5)) {
    	pp1 = shiftPoint(p1, e1);
    }

    if (pp0.y < 0 || pp1.y < 0) {
#ifdef POLYGON_TRACE
    	::dlog(LOG_NORMAL,"!!!!%.4f %.4f %.4f %.4f",pp0.x,pp0.y,pp1.x,pp1.y);
#endif
    	return -1;
    }
#ifdef POLYGON_TRACE
	::dlog(LOG_NORMAL, "split edges:%d %d\n", e0, e1);
	pp0.dlog(LOG_NORMAL);pp1.dlog(LOG_NORMAL);
	dlog(LOG_NORMAL);
#endif
    if (sameEdge(pp0, pp1)) {
#ifdef POLYGON_TRACE
		::dlog(LOG_NORMAL,"can't split: pp same edge.\n");
#endif
    	return -1;
    }

	vector<Point> vertex0;
	for (int i=0; i<=e0; i++)
		vertex0.push_back(vertex[i]);
	Point lastv = vertex0[vertex0.size()-1];
	if (p0.x != lastv.x || p0.y != lastv.y)
		vertex0.push_back(pp0);
	//if (getVertex(p2) < 0)
		vertex0.push_back(pp1);
	for (int i=e1+1; i<vertex.size(); i++)
		if (p1.x != vertex[i].x || p1.y != vertex[i].y)
			vertex0.push_back(vertex[i]);

	pg0.setVertexExact(vertex0);
#ifdef POLYGON_TRACE
::dlog(LOG_NORMAL,"pg1");pg0.dlog(LOG_NORMAL);
#endif
	pg0.Normalize();
#ifdef POLYGON_TRACE
pg0.dlog(LOG_NORMAL);
#endif
	if (pg0.vertex.size() < 3)
		return -1;

	vector<Point> vertex1;
	vertex1.push_back(pp0);
	for (int i=e0+1; i<=e1; i++) {
		if (vertex[i].x != vertex1[0].x || vertex[i].y != vertex1[0].y)
			vertex1.push_back(vertex[i]);
	}
	lastv = vertex1[vertex1.size()-1];
	if (p1.x != lastv.x || p1.y != lastv.y)
		vertex1.push_back(pp1);
	pg1.setVertexExact(vertex1);
#ifdef POLYGON_TRACE
::dlog(LOG_NORMAL,"pg2");pg1.dlog(LOG_NORMAL);
#endif
	pg1.Normalize();
#ifdef POLYGON_TRACE
pg1.dlog(LOG_NORMAL);
#endif
	if (pg1.vertex.size() < 3)
		return -1;

	pg0.id = id;
	pg0.id.append(0);
	pg1.id = id;
	pg1.id.append(1);
	return 0;
}

int Polygon::sameEdge(Point p0, Point p1)
{
	vector<int> e0 = findEdges(p0);
	vector<int> e1 = findEdges(p1);
#ifdef POLYGON_TRACE
	::dlog(LOG_NORMAL, "sameEdge %.2f,%.2f %.2f,%.2f",p0.x,p0.y,p1.x,p1.y);
	::dlog(LOG_NORMAL,"\nse_e1=");
	for (int i=0; i<e0.size(); i++) {
		::dlog(LOG_NORMAL,"%d-",e0[i]);
	}
	::dlog(LOG_NORMAL,"\nse_e2=");
	for (int i=0; i<e1.size(); i++) {
		::dlog(LOG_NORMAL,"%d-",e1[i]);
	}
	::dlog(LOG_NORMAL,"\n");
#endif
	for (int i=0; i<e0.size(); i++) {
		bool found = true;
		for (int j=0; j<e1.size(); j++)
			if (e0[i] != e1[j]) {
				found = false;
				break;
			}
		if (!found)
			return false;
	}
	return true;
}

vector<int> Polygon::findEdges(Point p)
{
	vector<int> edges;
//	if (!isMargin(p))
//		return edges;
	int v = getVertex(p);
	if (v >= 0) {
		edges.push_back(v);
		if (v > 0)
			edges.push_back(v-1);
		else
			edges.push_back(vertex.size()-1);
		return edges;
	}
	Cfloat dmin = img->getWidth()*img->getWidth() + img->getHeight()*img->getHeight();
	for (int i=0; i<vertex.size(); i++) {
		Point v0 = vertex[i];
		Point v1;
		v1 = vertex[(i+1)%vertex.size()];
//		Cfloat d = fabs((v1.y-v0.y)*p.x - (v1.x-v0.x)*p.y + v1.x*v0.y - v1.y*v0.x)/
//				sqrt((v1.y-v0.y)*(v1.y-v0.y)+(v1.x-v0.x)*(v1.x-v0.x));
		Point cl = p.ClosesetOnLine(v0, v1);
		Cfloat d;
		if (cl.Between(v0, v1))
			d = cl.Distance(p);
		else {
			Cfloat d0 = p.Distance(v0);
			Cfloat d1 = p.Distance(v1);
			d = d0 < d1 ? d0 : d1;
		}
#ifdef POLYGON_TRACE
//		::dlog(LOG_VERBOSE, "[(%.2f, %.2f) <-> %.2f] = %.3f\n",p.x,p.y,i,d);
#endif
		if (d <= MARGIN_DIFF + EPS) {
//			if (d < dmin) {
				dmin = d;
				edges.push_back(i);
//			}
		}
	}
//	edges.push_back(nmin);
	return edges;
}

int Polygon::findEdge(Point p)
{
	int edge = -1;
//	if (!isMargin(p))
//		return edges;
	int v = getVertex(p);
	if (v >= 0) {
		return v;
/*		edges.push_back(v);
		if (v > 0)
			edges.push_back(v-1);
		else
			edges.push_back(vertex.size()-1);
		return edges;
		*/
	}
	Cfloat dmin = img->getWidth()*img->getWidth() + img->getHeight()*img->getHeight();
	for (int i=0; i<vertex.size(); i++) {
		Point v0 = vertex[i];
		Point v1 = vertex[(i+1) % vertex.size()];
		Point cl = p.ClosesetOnLine(v0, v1);
		Cfloat d;
		if (cl.Between(v0, v1))
			d = cl.Distance(p);
		else {
			Cfloat d0 = p.Distance(v0);
			Cfloat d1 = p.Distance(v1);
			d = d0 < d1 ? d0 : d1;
		}
#ifdef POLYGON_TRACE
//		::dlog(LOG_VERBOSE, "[(%.2f, %.2f) <-> %.2f] = %.3f\n",p.x,p.y,i,d);
#endif
		if (d <= MARGIN_DIFF + EPS) {
			if (d < dmin) {
				dmin = d;
				edge = i;
			}
		}
	}
//	edges.push_back(nmin);
	return edge;
}

void Polygon::Normalize()
{
	int n0, n1, n2;
	bool erased = false;
	bool done;
	do {
		if (vertex.size() < 3)
			return;
		done = true;
		for (n0=0; n0<vertex.size(); n0++) {
			n1 = (n0 + 1) % vertex.size();
			n2 = (n0 + 2) % vertex.size();
			Point v0 = vertex[n0];
			Point v1 = vertex[n1];
			Point v2 = vertex[n2];
			if (FIS_EQ(v0.x, v1.x))
				done = !FIS_EQ(v2.x, v0.x);
			else
				done = !FIS_EQ(v2.y, v0.y + (v1.y-v0.y)/(v1.x-v0.x)*(v2.x-v0.x));
			if (!done) {
#ifdef POLYGON_TRACE
				::dlog(LOG_NORMAL,"\nNORMALIZE\n");
#endif
				int center = 0;
				if ((v0.y < v1.y && v1.y < v2.y) || (v2.y < v1.y && v1.y < v0.y))
					center = 1;
				else if ((v1.y < v0.y && v0.y < v2.y) || (v2.y < v0.y && v0.y < v1.y))
					center = 0;
				else if ((v0.y < v2.y && v2.y < v1.y) || (v1.y < v2.y && v2.y < v0.y))
					center = 2;
				else {
					if ((v0.x < v1.x && v1.x < v2.x) || (v2.x < v1.x && v1.x < v0.x))
						center = 1;
					else if ((v1.x < v0.x && v0.x < v2.x) || (v2.x < v0.x && v0.x < v1.x))
						center = 0;
					else if ((v0.x < v2.x && v2.x < v1.x) || (v1.x < v2.x && v2.x < v0.x))
						center = 2;
				}
				center = 1;
				vertex.erase(vertex.begin() + (n0 + center)%vertex.size());
				erased = true;
				break;
			}
		}
	} while (!done);
	if (erased) {
		computeMargin();
		computeFullMargin();
	}
}

bool Polygon::isMargin(Point p)
{
	if (!isInside(p))
		return false;
	if (p.x <= 0.5 || p.x >= img->getWidth()-0.5+DLIMIT-EPS || p.y <= 0.5 || p.y >= img->getHeight()-0.5+DLIMIT-EPS)
		return true;
	vector<int> edges = findEdges(p);
return edges.size() > 0;
	if (p.x > 1) {
		if (!isInside(p.x-1, p.y))
			return true;
		if (p.y > 1)
			if (!isInside(p.x-1, p.y-1))
				return true;
		if (p.y < img->getHeight()-1)
			if (!isInside(p.x-1, p.y+1))
				return true;
	}
	if (p.y > 1) {
		if (!isInside(p.x, p.y-1))
			return true;
		if (p.x < img->getWidth() - 1)
			if (!isInside(p.x+1, p.y-1))
				return true;
	}
	if (p.x > img->getWidth()-1) {
		if (!isInside(p.x+1, p.y))
			return true;
		if (p.y < img->getHeight()-1)
			if (!isInside(p.x+1, p.y+1))
				return true;
	}
	if (p.y < img->getHeight()-1)
		if (!isInside(p.x, p.y+1))
			return true;
	return false;
}


int Polygon::getMarginLen()
{
	return margin.size();
}

Point Polygon::getMarginPoint(int m)
{
	return margin[m];
}

int Polygon::getMarginNum(Point p)
{
	for (int i=0; i<margin.size(); i++)
		if (margin[i] == p)
			return i;
	return -1;
}

int Polygon::getFullMarginLen()
{
	return full_margin.size();
}

Point Polygon::getFullMarginPoint(int m)
{
	return full_margin[m];
}

int Polygon::getFullMarginNum(Point p)
{
	for (int i=0; i<full_margin.size(); i++)
		if (p.x > full_margin[i].x-DLIMIT-EPS && p.x < full_margin[i].x+DLIMIT+EPS &&
				p.y > full_margin[i].y-DLIMIT-EPS && p.y < full_margin[i].y+DLIMIT+EPS)
			return i;
	return -1;
}

int Polygon::getArea()
{
	int area = 0;
	for (Cfloat x=dim0.x; x<=dim1.x; x++)
		for (Cfloat y=dim0.y; y<=dim1.y; y++) {
			Point p = Point(x, y);
			if (isInside(p))
				area++;
		}
	return area;
}

bool Polygon::MarginIsInside(Point p, bool horiz)
{
#ifdef POLYGON_TRACE
	::dlog(LOG_ALL,"\nMarginIsInside ");
	p.dlog(LOG_ALL);
#endif
	if (!horiz && p.y < img->getHeight()+0.5+DLIMIT+EPS) {
		return /*getVertex(Point(p.x, p.y+DEDGE_VERT)) >= 0 || */isInside(Point(p.x, p.y+DEDGE_VERT));
	} else if (p.x < img->getWidth()-0.5+DLIMIT+EPS) {
		return /*getVertex(Point(p.x+DEDGE_HOR, p.y))>=0 || */isInside(Point(p.x+DEDGE_HOR, p.y));
	} else
		return /*getVertex(Point(p.x, p.y-DEDGE_VERT)) >= 0 || */isInside(Point(p.x, p.y-DEDGE_VERT));
}

bool Polygon::isInside(Point p)
{
#ifdef POLYGON_TRACE
	::dlog(LOG_ALL,"\nisInside");
	p.dlog(LOG_ALL);
	dlog(LOG_ALL);
	::dlog(LOG_ALL,"doIsInside");
#endif
	if (vertex.size() < 3)
		return false;//p.x == vertex[0].x && p.y == vertex[0].y;
	for (int i=0; i<pinside.size(); i++) {
		if (pinside[i] == p) {
::dlog(LOG_ALL, "pins");
			return true;
		}
	}
	if (dim0.x-EPS > p.x || dim1.x+EPS < p.x || dim0.y-EPS > p.y || dim1.y+EPS < p.y) {
		dim0.dlog(LOG_ALL);
		dim1.dlog(LOG_ALL);
		return false;
	}

	for (int i=0; i<poutside.size(); i++) {
		if (poutside[i] == p) {
			::dlog(LOG_ALL, "pout");
			return false;
		}
	}


	int intersect = 0;
	Cfloat x = p.x;
	Cfloat y = p.y;
	for (int i=0; i<vertex.size(); i++) {
		Cfloat x0 = vertex[i].x;
		Cfloat y0 = vertex[i].y;
		Cfloat x1, y1;
		if (i < vertex.size() - 1) {
			x1 = vertex[i+1].x;
			y1 = vertex[i+1].y;
		} else {
			x1 = vertex[0].x;
			y1 = vertex[0].y;
		}
		Cfloat x2, y2;
		if (i > 0)  {
			y2 = vertex[i-1].y;
			x2 = vertex[i-1].x;
		} else {
			x2 = vertex[vertex.size() - 1].x;
			y2 = vertex[vertex.size() - 1].y;
		}
#ifdef POLYGON_TRACE
		Point(x0, y0).dlog(LOG_ALL);
		::dlog(LOG_ALL, "-");
		Point(x1, y1).dlog(LOG_ALL);
#endif

	if (FIS_EQ(x, x0) && FIS_EQ(y, y0)) {
#ifdef POLYGON_TRACE
		::dlog(LOG_ALL, "\\/V");
#endif
		return MarginIsInside(p);
/*		bool belongs = false;
		belongs |= ((x1 > x) && (y1 <= y && (y2 > y || (y2 == y && p.y == img->getHeight()-1))));
		belongs |= (x2 > x && (y2 <= y && (y1 > y || (y1 == y && p.y == img->getHeight()-1))));
		belongs |= (((x1 == img->getWidth()-1 && y1 >= y) || (x2 == img->getWidth()-1 && y2 >= y)) && (p.x == img->getWidth()-1));
		return belongs;
		return ((x1 > x || x2 > x || ((x1 == x || x2 == x) && (p.x == img->getWidth()-1))) &&
				((y1 <= y && (y2 > y || (y2 == y && p.y == img->getHeight()-1))) ||
				 (y2 <= y && (y1 > y || (y1 == y && p.y == img->getHeight()-1)))));//true;
		 */
	}
	if (FIS_EQ(x, x1) && FIS_EQ(y, y1)) {
#ifdef POLYGON_TRACE
		::dlog(LOG_ALL, "\\/V");
#endif
		return MarginIsInside(p);
	}

		Cfloat yi;
		if (!FIS_EQ(x1, x0)) {
			yi = y0 + (y1-y0)/(x1-x0)*(x-x0);
			if (FIS_EQ(y1, y0))
				yi = y0;
		} else {
			if (FIS_EQ(x, x0)) {
				if ((y >= y0 && (y < y1/* || (FIS_EQ(y, y1) && p.y >= img->getHeight()-0.5)*/)) ||
						((y >= y1 && (y < y0 /*|| (FIS_EQ(y, y0) && p.y >= img->getHeight()-0.5)*/)))) {
/*					if (p.x >= img->getWidth()-0.5) {
#ifdef POLYGON_TRACE
						::dlog(LOG_ALL, "iz%d", x2<img->getWidth()-0.5-EPS);
#endif
						return x2 < img->getWidth()-0.5-EPS;
//						return MarginIsInside(p);
					} else */{
						return MarginIsInside(p, true);
					}
				} else {

					if (y >= y0 && y < y1) {
#ifdef POLYGON_TRACE
						::dlog(LOG_ALL, "iv");
#endif
						intersect++;
					}
					continue;

				}
			} else
				continue;
		}

#ifdef POLYGON_TRACE
#ifdef FLOAT128
		char buf[512] = {0};
		quadmath_snprintf(buf, 100, "%.15Qf", yi);
		::dlog(LOG_ALL, "yi=%s ", buf);
#else
		::dlog(LOG_ALL, "yi=%.13f ", yi);
#endif
#endif
		if (yi <= y-EPS/* && !(yi >= y && p.y >= img->getHeight()-0.5-EPS)*/)
			continue;
		else if (yi <= y+EPS/* && !(yi >= y && p.y >= img->getHeight()-0.5-EPS)*/) {
			if (!FIS_EQ(y0, y1))
				return MarginIsInside(p);
			else
				continue;
		}

		if (FIS_EQ(x, x0)) {
//			return MarginIsInside(p, true);
//if (x==x0){
#ifdef POLYGON_TRACE
			::dlog(LOG_ALL, "\\/(x1=%.3f, x2=%.3f)", x1, x2);
#endif
			if (FIS_EQ(x1, x0) || FIS_EQ(x2, x0)) {
				if (x0 < img->getWidth() - 0.5 - EPS)
					return MarginIsInside(p, true);
				else {
					if (FIS_EQ(x1, x0))
						return (y0 <= y && y1 > y) || (y1 <= y && y0 > y);
					else if (FIS_EQ(x2, x0))
						return (y0 <= y && y2 > y) || (y2 <= y && y0 > y);
				}
			}
			if (((x1 >= x && x2 >= x) || (x1 <= x && x2 <= x))/* ||
					(x >= img->getWidth()-0.5 && (y1>y && y2>y))*/)
				continue;
			else {
#ifdef POLYGON_TRACE
				::dlog(LOG_ALL, "invx\t");
#endif
				intersect++;
				continue;
			}
		}
		if (FIS_EQ(x, x1)) {
			if (FIS_EQ(y, y1))
				continue; // next cycle x==x0 && y==y0
			continue;
		}

//		::dlog(LOG_ALL, "[%1.f,%.1f]", x, yi);
		if (((yi >= y0 && yi <= y1) || (yi >= y1 && yi <= y0)) &&
				((x >= x0 && x <= x1) || (x >= x1 && x <= x0))) {
#ifdef POLYGON_TRACE
			::dlog(LOG_ALL, "i");
#endif
/*			if (y + 0.5 >= yi)
				intersect += MarginIsInside(point(x,y));
			else
*/				intersect++;
		}
	}
#ifdef POLYGON_TRACE
	::dlog(LOG_ALL, "\ti%d", intersect );
#endif
	return intersect % 2;
}

int Polygon::getVertex(Point p)
{
	for (int i=0; i<vertex.size(); i++)
		if (p.x > vertex[i].x - DLIMIT && p.x < vertex[i].x + DLIMIT &&
				p.y > vertex[i].y - DLIMIT && p.y < vertex[i].y + DLIMIT)
			return i;
	return -1;
}

string Polygon::print()
{
	string str = "polygon";
	str += getID().print();
	str += ": ";
	for (int i=0; i<vertex.size(); i++) {
		str += vertex[i].print();
	}

	str += "[";
	str += std::to_string(pinside.size());
	str += "-";
	str += std::to_string(poutside.size());
	str += "]";

	str += "\n";
	return str;
}

void Polygon::dlog(int level)
{
	if (!is_dlog(level))
		return;
if (0) {
	::dlog(level, "margin:\n");
	for (int i=0; i<margin.size(); i++)
		::dlog(level, "(%.3f, %.3f) ", (Cfloat)margin[i].x, (Cfloat)margin[i].y);
}
	::dlog(level, print().c_str());
}

Cfloat Point::Distance(Point p1)
{
	return sqrt(pow(p1.x-x,2)+pow(p1.y-y,2));
}

bool Point::Between(Point p1, Point p2)
{
	if (((x > p1.x-EPS && x < p2.x+EPS) || (x > p2.x-EPS && x < p1.x+EPS)) &&
			((y > p1.y-EPS && y < p2.y+EPS) || (y > p2.y-EPS && y < p1.y+EPS)))
		return true;
	else
		return false;
}

Point Point::ClosesetOnLine(Point p0, Point p1)
{
	if (*this == p0)
		return p0;
	if (*this == p1)
		return p1;
	if (FIS_EQ(p0.x, p1.x))
		return Point(p0.x, y);
	if (FIS_EQ(p1.y, p0.y))
		return Point(x, p0.y);
	Cfloat a = (p1.y-p0.y)/(p1.x-p0.x);
	Cfloat b = p0.y-a*p0.x;
	Cfloat cx = (y+x/a-b)/(a+1/a);
	Cfloat cy = a*cx+b;
	return Point(cx,cy);
}

bool Point::operator==(const Point &p) const
{
	return FIS_EQ(x, p.x) && FIS_EQ(y, p.y);
}

string Point::print()
{
	char buf[512]={0};
#ifdef FLOAT128
	char bx[100]={0};
	char by[100]={0};
	quadmath_snprintf(bx, 100, "%.25Qf", x);
	quadmath_snprintf(by, 100, "%.25Qf", y);
	snprintf(buf, 512, "(%s, %s)",bx,by);
#else
	snprintf(buf, 512, "(%f, %f)", x, y);
#endif
	return buf;
}

void Point::dlog(int level)
{
	if (!is_dlog(level))
		return;
	::dlog(level, print().c_str());
}
