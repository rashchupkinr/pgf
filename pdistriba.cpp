#include "pdistriba.h"

#include <limits.h>
#include <math.h>
#include "logging.h"

PDistribA PDistribA::NullPD;
int PDistribA::UMaxVal;
PDistribA PDistribA::UniformPD;
pair<long, double> PDistribA::nullp;

PDistribA::PDistribA()
{
    nullp = make_pair(0, -1);
}

PDistribA::PDistribA(map<long, double> _pdistrib)
: PDistribA()
{
    pdistrib = _pdistrib;
}

void PDistribA::normalize()
{
    double P = 0;
    for (auto it=pdistrib.begin(); it!=pdistrib.end(); it++)
        P += it->second;
    for (auto it=pdistrib.begin(); it!=pdistrib.end(); it++)
        it->second /= P;
//	::dlog(LOG_NORMAL, "P %f\n", P);
    sort();
}

void PDistribA::sort(bool inverse)
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

void PDistribA::reset()
{
    pdistrib.clear();
    psorted.clear();
}

void PDistribA::set(long v, double p)
{
    pdistrib[v] = p;
}

double PDistribA::get(long v) const
{
    auto it = pdistrib.find(v);
    if (it == pdistrib.end())
        return 0;
    return pdistrib.at(v);
}

int PDistribA::getSize() const
{
    return psorted.size();
}

const pair<long, double> PDistribA::operator[](std::size_t i) const
{
    if (i >= psorted.size())
        return nullp;
    return psorted[i];
}

const PDistribA PDistribA::operator+(const PDistribA &other) const
{
    PDistribA sum;

    for (auto it=pdistrib.begin(); it!=pdistrib.end(); it++)
        sum.set((*it).first, (*it).second);
    const map<long, double> *otherPD = other.getPDistribA();
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

float PDistribA::getMedProb() const
{
    float length = 0;
    for (int i=0; i<getSize(); i++)
        length += (*this)[i].second;
    return length / getSize();
}

void PDistribA::setUniformPD(int MaxVal)
{
    if (MaxVal == UMaxVal) {
        *this = UniformPD;
        return;
    }
    for (int i=0; i<=MaxVal; i++)
        set(i, 1.0/MaxVal);
//	uniformPD.sort();
}

extern int _DLOG_cur;
extern int LogPriority;

string PDistribA::print() const
{
    if (_DLOG_cur > LogPriority)
        return "";
    string str = "pdistrib[";
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
