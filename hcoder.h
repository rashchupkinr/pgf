/*
 * hcoder.h
 *
 *  Created on: 20.06.2015
 *      Author: roman
 */

#ifndef BAR_H_
#define BAR_H_
#include "pdistrib.h"
#include "bitarr.h"

class hcoder {
public:
	hcoder();
	virtual ~hcoder();
	// pd must be sorted
	int setPDistrib(PDistrib pd, bool _optimist = false);
	void reset();
	bitarr encode(long val);
	bitarr encodeTbl(long val);
	int decode(bitarr *arr, int pos, long *value);
	void dump(int priority);
	void bench();
	void test();
protected:
	class Vertex {
	public:
		Vertex(vector<Vertex *> *tree)	:p(0),s0(0),s1(0)	{	tree->push_back(this);	}
		int countN()	{	int n=0; if (p) n++; if(s0) n++; if (s1) n++;	return n;	}
		unsigned int depth;
		bitarr nominal;
		bool busy;
		long value;
		Vertex *p, *s0, *s1;
	};
	Vertex *findLocalRoot(unsigned int depth);
	int raiseBTree(Vertex *v, int diff);
	int adjustBTree();
	int genBTree();
	void clearBTree();
	int genCodes();
private:
	PDistrib pd;
	bool optimist;
	vector<Vertex *> btree;
	std::map<long, bitarr> codes;
};

#endif /* BAR_H_ */
