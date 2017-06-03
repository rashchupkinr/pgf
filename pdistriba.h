#ifndef PDISTRIBA_H
#define PDISTRIBA_H

#include <vector>
#include <map>
#include "pdistrib.h"
#include "logging.h"

using namespace std;

class PDistribA : public PDistrib {
public:
    PDistribA();
    PDistribA(map<long, double> _PDistribA);
    virtual void normalize();
    virtual void sort(bool inverse = false);
    void reset();
    virtual void set(long v, double p);
    virtual double get(long v) const;
    virtual int getSize() const;
    const pair<long, double> operator[](std::size_t i) const;
    const map<long, double> *getPDistribA() const	{	return &pdistrib;	}
    virtual const PDistribA operator+(const PDistribA &other) const;	// must be normalized
    float getMedProb() const;

    void setNullPD()	{	*this = NullPD;	}
    void setUniformPD(int MaxVal);

    virtual string print() const;
    static void initUniformPD(int MaxVal) {
        UMaxVal = MaxVal;
        for (int i=0; i<=MaxVal; i++)
            UniformPD.set(i, 1.0/MaxVal);
        UniformPD.sort();
        UniformPD.normalize();
    }
private:
    map<long, double> pdistrib;
    vector<pair<long, double> > psorted;
    static pair<long, double> nullp;
    static PDistribA NullPD;
    static PDistribA UniformPD;
    static int UMaxVal;
};


#endif // PDISTRIBA_H
