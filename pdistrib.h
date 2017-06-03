#ifndef __PDISTRIB__
#define __PDISTRIB__

#include <string>

using namespace std;

class PDistrib {
public:
    virtual void normalize() = 0;
    virtual void sort(bool inverse = false) = 0;
    virtual void reset() = 0;
    virtual void set(long v, double p) = 0;
    virtual double get(long v) const = 0;
    virtual int getSize() const = 0;
    virtual float getMedProb() const = 0;
    virtual void addSpikeUniform(long V, unsigned long radius, double P);
    virtual void addSpikeLinear(long V, unsigned long radius, double P);
    virtual void addSpikeEllipse(long V, unsigned long radius, double P);

    virtual void setNullPD() = 0;
    virtual void setUniformPD(int MaxVal) = 0;

    virtual const pair<long, double> operator[](std::size_t i) const = 0;
    virtual std::string print() const = 0;
};
#endif
