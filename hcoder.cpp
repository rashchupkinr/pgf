/*
 * hcoder.cpp
 *
 *  Created on: 20.06.2015
 *      Author: roman
 */

#include <math.h>
#include "hcoder.h"
#include "logging.h"
#undef BAR_TRACE

int hcoder::setPDistrib(PDistrib _pd, bool _optimist)
{
	reset();
	pd = _pd;
	pd.normalize();
	optimist = _optimist;
	return genBTree();
}

void hcoder::reset()
{
	pd.reset();
	codes.clear();
	clearBTree();
}

int hcoder::raiseBTree(Vertex *v, int diff)
{
#ifdef BAR_TRACE
	cout<<"raise val="<<v->value<<v->nominal<<"d["<<v->depth<<"]";
#endif
	bitarr *n = &v->nominal;
	n->cut(n->size() - diff);
	v->depth--;
#ifdef BAR_TRACE
	cout << "->" << v->nominal<<endl;
#endif
	if (v->s0)
		raiseBTree(v->s0, diff + 1);
	if (v->s1)
		raiseBTree(v->s1, diff + 1);
	return 0;
}

int hcoder::adjustBTree()
{
	bool done;
	do {
		done = true;
		for (int i=0; i<btree.size(); i++) {
			Vertex *v = btree[i];
			int nvnum = v->countN();
			if ((v->depth > 0 && nvnum == 2) || (v->depth == 0 && nvnum == 1)) {
#ifdef BAR_TRACE
				cout << "adjust" << v->nominal<<"d["<<v->depth<<"]"<<endl;
#endif
				if (v->s0) {
					v->s0->p = v->p;
					raiseBTree(v->s0, 1);
				}
				if (v->s1) {
					v->s1->p = v->p;
					raiseBTree(v->s1, 1);
				}
				btree.erase(btree.begin()+i);
				delete v;
				done = false;
				break;
			}
		}
	} while (done == false);
	return 0;
}

hcoder::Vertex *hcoder::findLocalRoot(unsigned int depth)
{
	Vertex *localroot = 0;
	for (int i=0; i<btree.size(); i++) {
		Vertex *v = btree[i];
		int nvnum = v->countN();
		if (optimist) {
			if ((localroot == 0 || localroot->depth < v->depth) && (v->depth < depth) &&
					((v->depth == 0 && nvnum < 2) || (v->depth > 0 && nvnum < 3)) &&
					!v->busy)
				localroot = v;
		} else
			if ((localroot == 0 || localroot->depth > v->depth) && (v->depth < depth) &&
					((v->depth == 0 && nvnum < 2) || (v->depth > 0 && nvnum < 3)) &&
					!v->busy)
				localroot = v;
	}
	return localroot;
}

int hcoder::genBTree()
{
	pd.sort(optimist);

	Vertex *localroot = new Vertex(&btree);
	localroot->depth = 0;
	localroot->busy = false;
//	btree.push_back(localroot);
	for (int i=0; i<pd.getSize(); i++) {
		int val = pd[i].first;
		double p = pd[i].second;
		int depth = ceil(-log2(p));
#ifdef BAR_TRACE
		dlog(LOG_ERROR,"val%d p%f depth%d\n",val,p,depth);
#endif
		localroot = findLocalRoot(depth);
		if (localroot == 0) {
			dlog(LOG_ERROR, "BAR error: localroot[%d:%.3f] == 0.\n", val, p);
			return -1;
		}
#ifdef BAR_TRACE
		cout <<"lr"<<localroot->nominal<<"N"<<localroot->countN()<<"d["<<localroot->depth<<"]";
#endif
		bool dir = 0;
		if (localroot->s0 && !localroot->s1)
			dir = 1;
		else if (!localroot->s0 && localroot->s1)
			dir = 0;
		else if (!localroot->s0 && !localroot->s1)
			dir = optimist;
		else {
			dlog(LOG_ERROR, "BAR error: wrong localroot.\n");
			return -1;
		}

		if (localroot->depth == depth) {
			localroot->busy = false;
			cout << "cont"<<endl;
			continue;
		}

		Vertex *pv = localroot;
		Vertex *v = 0;
		bitarr cba = localroot->nominal;
		for (int d = localroot->depth+1; d<=depth; d++) {
			v = new Vertex(&btree);
			v->p = pv;
			if (d == localroot->depth+1) {
				cba.append(dir);
				if (dir)
					pv->s1 = v;
				else
					pv->s0 = v;
			} else {
				cba.append(0);
				pv->s0 = v;
			}
			v->depth = d;
			v->nominal = cba;
			v->busy = false;
#ifdef BAR_TRACE
			cout<<"edge"<<pv->nominal<<"--"<<v->nominal<<"d["<<d<<"]"<<endl;
#endif
			pv = v;
		}
		if (v) {
			v->busy = true;
			v->value = val;
		}
	}
	return adjustBTree();
}

void hcoder::clearBTree()
{
	for (int i=0; i<btree.size(); i++) {
		Vertex *v = btree[i];
		if (v)
			delete v;
	}
	btree.clear();
}

int hcoder::genCodes()
{
	for (int i=0; i<btree.size(); i++) {
		Vertex *v = btree[i];
		if (!v->busy)
			continue;
		codes[v->value] = v->nominal;
	}
	return 0;
}

bitarr hcoder::encode(long val)
{
	for (int i=0; i<btree.size(); i++) {
		Vertex *v = btree[i];
		if (!v)
			continue;
		if (!v->busy)
			continue;
		if (v->value == val)
			return v->nominal;
	}
	return bitarr();
}

bitarr hcoder::encodeTbl(long val)
{
	return codes[val];
}

int hcoder::decode(bitarr *arr, int pos, long *value)
{
	for (int i=0; i<btree.size(); i++) {
		Vertex *v = btree[i];
		if (!v)
			continue;
		if (!v->busy)
			continue;
		bitarr code = v->nominal;
		if (arr->size() < pos + code.size())
			continue;
		int j;
		for (j=0; j<code.size(); j++) {
			if (arr->get(pos+j) != code.get(j))
				break;
		}
		if (j == code.size()) {
			*value = v->value;
			return code.size();
		}
	}
	return 0;
}

void hcoder::dump(int priority)
{
	for (int i=0; i<pd.getSize(); i++) {
		dlog(priority ,"val=%d p=%.5f %s\n", pd[i].first, pd[i].second, encode(pd[i].first).print().c_str());
//		cout << "val="<<pd[i].first<<" p="<<pd[i].second<<" "<<encode(pd[i].first)<<endl;
	}
}

void hcoder::bench()
{
	double theor = 0;
	double med = 0;
	for (int i=0; i<pd.getSize(); i++) {
//	for (auto it=pd.begin(); it!=pd.end(); it++) {
		theor += log2(1/pd[i].second)*pd[i].second;
		med += encode(pd[i].first).size()*pd[i].second;
	}
	cout << "Theor=\t"<<theor<<endl;
	cout << "Med=\t"<<med<<endl;

}

void hcoder::test()
{
	srandom(time(0));
	PDistrib pd;
	for (int i=0; i<16; i++) {
		if (i==0)
			pd.set(i, 1.0/4);
		else if (i==1)
			pd.set(i, 1.0/6);
		else if (i<32)
			pd.set(2*i, 1.0/(6+2*abs(i)));
		else
			pd.set(2*i, 1.0/256);
	}

	setPDistrib(pd, true);
	for (int i=0; i<pd.getSize(); i++) {
		dlog(LOG_ERROR,"%d:%.02f ", pd[i].first, pd[i].second);
	}
	dlog(LOG_ERROR, "\n");
	dump(LOG_ERROR);
	bench();
}

hcoder::hcoder()
{
	// TODO Auto-generated constructor stub
}

hcoder::~hcoder()
{
	reset();
	// TODO Auto-generated destructor stub
}

