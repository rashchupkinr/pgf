/*
 * arcoder.cpp
 *
 *  Created on: 22.06.2015
 *      Author: roman
 */

#include "arcoder.h"
#include <limits.h>
#include <math.h>
#include <inttypes.h>
#include "hcoder.h"
#include "logging.h"

#define AR_TRACE

void arcoder::setOProb(map<uint64_t, double> _oprob)
{
	reset();
	oprob = _oprob;
	normalizeOProb();
	sortOProb();
	genRange();
}
void arcoder::reset()
{
	oprob.clear();
	range.clear();
	opsorted.clear();
	codes.clear();
}

void arcoder::normalizeOProb()
{
	double P = 0;
	for (auto it=oprob.begin(); it!=oprob.end(); it++)
		P += it->second;
	for (auto it=oprob.begin(); it!=oprob.end(); it++)
		it->second/=P;
	cout<<"P"<<P<<endl;
}

void arcoder::sortOProb()
{
	map<uint64_t, double>_oprob = oprob;
	uint64_t valmin, valmax;
	bool done = false;
	int a=0;
	do {
		done = true;
		double pmin = -1;
		for (auto it=_oprob.begin(); it!=_oprob.end(); it++) {
			if (it->second > pmin) {
				pmin = it->second;
				valmin = it->first;
				done = false;
			}
		}
		if (!done) {
			_oprob.erase(valmin);
			if (pmin > -1)
				opsorted.push_back(make_pair(valmin, pmin));
		}
	} while(!done);
}

void arcoder::genRange()
{
	range.push_back(0);
	for (int i=0; i<opsorted.size(); i++) {
		uint64_t base = 0;
		base = range[i];
		uint64_t end = base + UINT64_MAX*opsorted[i].second;
		if (base > 0 && end == 0)
			end = UINT64_MAX-1;
		range.push_back(end);
#ifdef AR_TRACE
//		dlog(LOG_ERROR, "%u/%02f:\trange %" PRIu64 "-%" PRIu64 "\n", opsorted[i].first, opsorted[i].second, base, end);
#endif
	}
}

bitarr arcoder::do_encode(uint64_t val)
{
	if (range.size() == 0)
		return bitarr();
	int idx;
	for (idx=0; idx<opsorted.size(); idx++)
		if (opsorted[idx].first == val)
			break;
	uint64_t base = range[idx];
	uint64_t end;
	if (idx < range.size() - 1)
		end = range[idx+1];
	else
		end = UINT64_MAX;

	uint64_t v = end - base;
	int width = 0;
	while ((end - base) >> width)
		width++;
	width--;
	bitarr cba;
	cba.append(base >> width, 64-width);
	return cba;
}

void arcoder::genCodes()
{

}

bitarr arcoder::encode(uint64_t val)
{
	if (codes.size())
		return codes[val];
	else
		return do_encode(val);
}

void arcoder::dump()
{
	for (int i=0; i<opsorted.size(); i++) {
		cerr << "val=" << opsorted[i].first << "p" << opsorted[i].second << encode(opsorted[i].first) << endl;
	}
}

void arcoder::bench()
{
	double theor = 0;
	double med = 0;
	for (auto it=oprob.begin(); it!=oprob.end(); it++) {
		if (it->second == 0)
			continue;
		theor += log2(1/it->second)*it->second;
		med += encode(it->first).size()*it->second;
	}
	cout << "Theor=\t"<<theor<<endl;
	cout << "Med=\t"<<med<<endl;
}

void arcoder::test()
{
	srandom(time(0));
	map<uint64_t, double> _oprob;
	for (int i=0; i<8; i++) {
		if (i==0)
			_oprob.insert(make_pair(i, 1.0/4));
		else if (i==1)
			_oprob.insert(make_pair(i, 1.0/6));
		else if (i<32)
			_oprob.insert(make_pair(2*i, 1.0/(6+2*i)));
		else
			_oprob.insert(make_pair(2*i, 1.0/256));
	}

	setOProb(_oprob);
	for (auto it=oprob.begin(); it!=oprob.end(); it++) {
		dlog(LOG_ERROR,"%d:%.02f ", it->first, it->second);
	}
	dlog(LOG_ERROR, "\n");
	dump();
	bench();
}

arcoder::arcoder() {
	// TODO Auto-generated constructor stub

}

arcoder::~arcoder() {
	// TODO Auto-generated destructor stub
}

