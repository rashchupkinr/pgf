#include "pdistribn.h"
#include "logging.h"
#include <math.h>

PDistribN PDistribN::NullPD;
int PDistribN::UMaxVal;
PDistribN PDistribN::UniformPD;

PDistribN::PDistribN()
{

}

void PDistribN::sort(bool inverse)
{
    psorted.clear();
    vector<double> _pdistrib = pdistrib;
    long valmin, valmax;
    bool done = false;
    int a=0;
    if (inverse) {
        do {
            done = true;
            double pmax = 99999;
            for (int i=0; i<_pdistrib.size(); i++){
                if (_pdistrib[i] < pmax) {
                    pmax = _pdistrib[i];
                    valmax = i;
                    done = false;
                }
            }
            if (!done) {
                _pdistrib[valmax] = 99999;
                if (pmax <= 99999)
                    psorted.push_back(make_pair(valmax, pmax));
            }
        } while(!done);
    } else {
        do {
            done = true;
            double pmin = -1;
            for (int i=0; i<_pdistrib.size(); i++) {
                if (_pdistrib[i] > pmin) {
                    pmin = _pdistrib[i];
                    valmin = i;
                    done = false;
                }
            }
            if (!done) {
                _pdistrib[valmin] = -1;
                if (pmin > -1)
                    psorted.push_back(make_pair(valmin, pmin));
            }
        } while(!done);
    }
}

void PDistribN::normalize()
{
    double P = 0;
    for (int i=0; i<pdistrib.size(); i++)
        P += pdistrib[i];
    for (int i=0; i<pdistrib.size(); i++)
        pdistrib[i] /= P;
//	::dlog(LOG_NORMAL, "P %f\n", P);
    //sort();
}
extern int _DLOG_cur;
extern int LogPriority;

string PDistribN::print() const
{
    if (_DLOG_cur > LogPriority)
        return "";
    string str = "PDistribN[";
    str += std::to_string(getSize());
    str += "]: ";
    char buf[512];
    double pmin = 100;
    for (int i=0; i<getSize(); i++) {
        if (pdistrib[i] < pmin)
            pmin = pdistrib[i];
    }
    for (int i=0; i<getSize(); i++) {
        if (pdistrib[i] == pmin)
            continue;
        sprintf(buf, "%d:%.3f ", i, pdistrib[i] );
        str += buf;
    }
    return str;
}

const PDistribN PDistribN::operator+(const PDistribN &other) const
{
    PDistribN sumPD;
    long s = getSize();
    if (other.getSize() > s)
        s = other.getSize();
    for (int i=0; i<s; i++) {
        if (pdistrib.size() > i)
            sumPD.set(i, get(i) + other.get(i));
        else
            sumPD.set(i, other.get(i));
    }
    return sumPD;
}

