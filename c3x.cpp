#include <stdlib.h>
#include <math.h>
#include "c3x.h"
using namespace std;

unsigned int c3x::dist(c3x x)
{
	float d = 0;
	d += (x.c0() - c0())*(x.c0() - c0());
	d += (x.c1() - c1())*(x.c1() - c1());
	d += (x.c2() - c2())*(x.c2() - c2());
	return round(sqrt(d));
}

vector<c3x> c3x::list_dist(int dist)
{
	unsigned int max = (1 << bit) - 1;
	vector<c3x> v;
	for (int x=c0()-dist; x<=c0()+dist; x++) {
		if (x < 0 || x > max)
			continue;
		for (int y=c1()-dist; y<=c1()+dist; y++) {
			if (x < 0 || x > max)
				continue;
			for (int z=c2()-dist; z<=c2()+dist; z++) {
				if (x < 0 || x > max)
					continue;
				v.push_back(c3x(bit, x, y, z));
			}
		}
	}
	return v;
}

string c3x::print() const
{
	string str = "{";
	str += std::to_string(c0());
	str += ",";
	str += std::to_string(c1	());
	str += ",";
	str += std::to_string(c2());
	str += "}";
	return str;
}
