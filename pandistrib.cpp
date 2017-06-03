#include <stdlib.h>
#include "pandistrib.h"
#include "logging.h"

int PAnDistrib::AddArea(c3x val, unsigned int radius)
{
//	c3area c = c3area(val, c3x(radius, radius, radius));
	//dlog(LOG_FATAL, "%d\n", areas.size());
	for (int i=0; i<areas.size(); i++) {
		dlog(LOG_FATAL, "%d %d\n", i, areas.size());
		continue;
	//	if (areas[i].isIntersect(c)) {
		//	c.Merge(areas[i], MaxRadius);
		//}
	}
}

std::string PAnDistrib::print()
{
	std::string s = "{";
	for (int i=0; i<areas.size(); i++)
		s += areas[i].print();
	s += "}";
	return s;
}
