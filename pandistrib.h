#ifndef __PANDISTRIB
#define __PANDISTRIB

#include <vector>
#include <string>
#include "c3x.h"


class PAnDistrib {
public:
	PAnDistrib(int _PNum, int _MinRadius, int _MaxRadius)
	:PNum(_PNum),
	 MinRadius(_MinRadius),
	 MaxRadius(_MaxRadius)
	{
	}
	int AddArea(c3x val, unsigned int radius);
	std::vector<c3area> getAreas()	{	return areas;	}
	std::string print();
private:
	int PNum;
	int MinRadius, MaxRadius;
	std::vector<c3area> areas;
};

#endif
