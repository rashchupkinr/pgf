/*
 * arcoder.h
 *
 *  Created on: 22.06.2015
 *      Author: roman
 */

#ifndef ARCODER_H_
#define ARCODER_H_
#include <vector>
#include <map>
#include "bitarr.h"

class arcoder {
public:
	arcoder();
	virtual ~arcoder();
	void setOProb(map<uint64_t, double> _oprob);
	void reset();
	bitarr encode(uint64_t val);
	void dump();
	void bench();
	void test();
	void genRange();
	void genCodes();
private:
	void normalizeOProb();
	void sortOProb();
	bitarr do_encode(uint64_t val);
	map<uint64_t, double> oprob;
	vector<pair<uint64_t, double> >opsorted;
	vector<uint64_t> range;
	std::map<uint64_t, bitarr> codes;
};

#endif /* ARCODER_H_ */
