#include <limits.h>
#include <math.h>
#include "pdistrib.h"
#include "logging.h"

PDistrib PDistrib::NullPD;
int PDistrib::UMaxVal;
PDistrib PDistrib::UniformPD;
pair<long, double> PDistrib::nullp;

PDistrib::PDistrib()
{
	nullp = make_pair(0, -1);
}

PDistrib::PDistrib(map<long, double> _pdistrib)
: PDistrib()
{
	pdistrib = _pdistrib;
}
void PDistrib::normalize()
{
	double P = 0;
	for (auto it=pdistrib.begin(); it!=pdistrib.end(); it++)
		P += it->second;
	for (auto it=pdistrib.begin(); it!=pdistrib.end(); it++)
		it->second /= P;
//	::dlog(LOG_NORMAL, "P %f\n", P);
	sort();
}

void PDistrib::sort(bool inverse)
{
	psorted.clear();
	map<long, double>_pdistrib = pdistrib;
	long valmin, valmax;
	bool done = false;
	int a=0;
	if (inverse) {
		do {
			done = true;
			double pmax = INT_MAX;
			auto it=_pdistrib.begin();
			for (; it!=_pdistrib.end(); it++) {
				if (it->second < pmax) {
					pmax = it->second;
					valmax = it->first;
					done = false;
				}
			}
			if (!done) {
				_pdistrib.erase(valmax);
				if (pmax <= INT_MAX)
					psorted.push_back(make_pair(valmax, pmax));
			}
		} while(!done);
 	} else {
		do {
			done = true;
			double pmin = -1;
			for (auto it=_pdistrib.begin(); it!=_pdistrib.end(); it++) {
				if (it->second > pmin) {
					pmin = it->second;
					valmin = it->first;
					done = false;
				}
			}
			if (!done) {
				_pdistrib.erase(valmin);
				if (pmin > -1)
					psorted.push_back(make_pair(valmin, pmin));
			}
		} while(!done);
 	}
}

void PDistrib::reset()
{
	pdistrib.clear();
	psorted.clear();
}

void PDistrib::set(long v, double p)
{
	pdistrib[v] = p;
}

double PDistrib::get(long v)
{
	auto it = pdistrib.find(v);
	if (it == pdistrib.end())
		return 0;
	return pdistrib[v];
}

int PDistrib::getSize() const
{
	return psorted.size();
}

const pair<long, double> &PDistrib::operator[](std::size_t i) const
{
	if (i >= psorted.size())
		return nullp;
	return psorted[i];
}

const PDistrib PDistrib::operator+(const PDistrib &other) const
{
	PDistrib sum;

	for (auto it=pdistrib.begin(); it!=pdistrib.end(); it++)
		sum.set((*it).first, (*it).second);
	const map<long, double> *otherPD = other.getPDistrib();
	for (auto ito=otherPD->begin(); ito!=otherPD->end(); ito++) {
		long v = (*ito).first;
		auto it = pdistrib.find(v);
		if (it == pdistrib.end())
			sum.set(v, (*ito).second);
		else
			sum.set(v, (*it).second + (*ito).second);
	}
//	sum.sort();
	return sum;
}

float PDistrib::getMedProb() const
{
	float length = 0;
	for (int i=0; i<getSize(); i++)
		length += (*this)[i].second;
	return length / getSize();
}

void PDistrib::addSpikeUniform(long V, unsigned long radius, double P)
{
	double baseP = P/(2*radius);
	int v = V-radius;
	if (v < 0)
		v = 0;
	for (;v<=V+radius; v++) {
		if (radius)
			set(v, get(v) + baseP);
		else
			set(v, get(v) + P);
	}
	sort();
}

void PDistrib::addSpikeLinear(long V, unsigned long radius, double P)
{
	double baseP = P/(2*radius);
	int v = V-radius;
	if (v < 0)
		v = 0;
	for (;v<=V+radius; v++) {
		if (radius) {
			double p = baseP * (1 - (double)abs(v-V)/radius);
			set(v, get(v) + p);
		} else
			set(v, get(v) + P);
	}
	sort();
}

void PDistrib::addSpikeEllipse(long V, unsigned long radius, double P)
{

	double baseP = P/(2*radius);
	int v = V-radius;
	if (v < 0)
		v = 0;
	for (;v<=V+radius; v++) {
		double p = sqrt((double)radius*radius - (v-V)*(v-V));
		if (radius)
			set(v, get(v) + p*baseP/radius);
		else
			set(v, get(v) + P);
	}
	sort();
}

PDistrib PDistrib::getUniformPD(int MaxVal)
{
	if (MaxVal == UMaxVal)
		return UniformPD;
	PDistrib uniformPD;
	for (int i=0; i<=MaxVal; i++)
		uniformPD.set(i, 1.0/MaxVal);
//	uniformPD.sort();
	return uniformPD;
}

string PDistrib::print() const
{
	string str = "PDistrib[";
	str += std::to_string(getSize());
	str += "]: ";
	char buf[512];
	for (int i=0; i<getSize(); i++) {
		sprintf(buf, "%d:%.3f ", (int)(*this)[i].first, (*this)[i].second);
		str += buf;
		if ((*this)[i].second == (*this)[getSize()-1].second)
			break;
	}
	return str;
}
