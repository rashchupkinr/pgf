#ifndef __C3X__
#define __C3X__

#include <vector>
#include <string>

class c3x {
public:
	c3x(unsigned int c1, unsigned int c2, unsigned int c3, int _bit = 8)
	: bit(_bit)
	{
		set(c1, c2, c3);
	}
	c3x(): c3x(0, 0, 0)	{}
	operator unsigned long() const {	return data;	}
	unsigned int c0() const	{	return (data >> 2*bit);					};
	unsigned int c1() const	{	return (data >> bit) & ((1<<bit)-1);	};
	unsigned int c2() const	{	return (data & ((1<<bit)-1));			};
	unsigned char ci(unsigned int k) const
	{
		switch (k) {
		case 0:
			return c0();
		case 1:
			return c1();
		case 2:
			return c2();
		}
		return -1;
	}
	void seti(int i, unsigned int ci) {
		switch (i) {
		case 0:
			 set(ci, c1(), c2());
		case 1:
			 set(c0(), ci, c2());
		case 2:
			 set(c0(), c1(), ci);
		}
	}
	void set(unsigned int c0, unsigned int c1, unsigned int c2)
	{
		data = 0;
		data = c0 << 2*bit;
		data |= c1 << bit;
		data |= c2;
	}
	float dist(c3x x);
	std::vector<c3x> list_dist(int dist);
	std::string print() const;
private:
	int bit;
	unsigned long data;
};


class c3area {
public:
	c3area(c3x c, c3x r)
		:center(c),
		 radius(r)
	{
	}
	c3area(): c3area(c3x(), c3x()) {}
	c3x center;
	c3x radius;
	bool isInside(c3x v) {
		if (v.c0() < center.c0() - radius.c0() || v.c0() > center.c0() + radius.c0())
			return false;
		if (v.c1() < center.c1() - radius.c1() || v.c1() > center.c1() + radius.c1())
			return false;
		if (v.c2() < center.c2() - radius.c2() || v.c2() > center.c2() + radius.c2())
			return false;
		return true;
	}
	bool isIntersect(c3area c) {
		if (c.center.c0() + c.radius.c0() < center.c0() - radius.c0() && c.center.c0() - c.radius.c0() > center.c0() + radius.c0())
			return false;
		if (c.center.c1() + c.radius.c1() < center.c1() - radius.c1() && c.center.c1() - c.radius.c1() > center.c1() + radius.c1())
			return false;
		if (c.center.c2() + c.radius.c2() < center.c2() - radius.c2() && c.center.c2() - c.radius.c2() > center.c2() + radius.c2())
			return false;
		return true;
	}
	void Merge(c3area a, int MaxRadius)
	{
		center = c3x(center.c0() + a.center.c0() / 2,
				center.c1() + a.center.c1() / 2,
				center.c2() + a.center.c2() / 2);

		int r0 = radius.c0() + a.radius.c0();
		if (r0 > MaxRadius)
			r0 = MaxRadius;
		int r1 = radius.c1() + a.radius.c1();
		if (r1 > MaxRadius)
			r1 = MaxRadius;
		int r2 = radius.c2() + a.radius.c2();
		if (r2 > MaxRadius)
			r2 = MaxRadius;
		radius = c3x(r0, r1, r2);
	}
	std::string print()
	{
		std::string s = "[";
		s += center.print();
		s += radius.print();
		s += "]";
		return s;
	}
};

#endif
