#ifndef __C3X__
#define __C3X__

#include <vector>
#include <string>

class c3x {
public:
	c3x(int _bit, unsigned int c1, unsigned int c2, unsigned int c3)
	: bit(_bit)
	{
		set(c1, c2, c3);
	}
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
	unsigned int dist(c3x x);
	std::vector<c3x> list_dist(int dist);
	std::string print() const;
private:
	int bit;
	unsigned long data;
};

#endif
