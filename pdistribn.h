#ifndef PDISTRIBN_H
#define PDISTRIBN_H
#include <vector>
#include "pdistrib.h"
using namespace std;

class PDistribN : public PDistrib
{
public:
    PDistribN();
    void set(long v, double p) {
        if (v >= pdistrib.size())
            pdistrib.resize(v+1);
        pdistrib[v] = p;
    }
    double get(long v) const {
        if (v > pdistrib.size())
            return 0;
        return pdistrib[v];
    }

    int getSize() const
    {
        return pdistrib.size();
    }
    void reset() { pdistrib.clear(); }
    float getMedProb() const { return 0; }
    void normalize();
    string print() const;
    void sort(bool inverse = false);
    const PDistribN operator+(const PDistribN &other) const;
    const vector<double> *getPDistribN() const	{	return &pdistrib;	}
    const pair<long, double> operator[](std::size_t i) const
    {
        return psorted[i];
        std::pair<long, double> p;
        p.first = i;
        p.second = pdistrib[i];
        return p;
    }

    void setNullPD()	{	pdistrib = *NullPD.getPDistribN();	}
    void setUniformPD(int MaxVal) { pdistrib = *UniformPD.getPDistribN();  }
    static void initUniformPDN(int MaxVal) {
        UMaxVal = MaxVal;
        for (int i=0; i<=MaxVal; i++)
            UniformPD.set(i, 1.0/MaxVal);
        UniformPD.sort();
        UniformPD.normalize();
    }

private:
    vector<double> pdistrib;
    vector<pair<long, double> > psorted;
    static PDistribN NullPD;
    static PDistribN UniformPD;
    static int UMaxVal;
};

#endif // PDISTRIBN_H
