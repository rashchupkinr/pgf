#include <math.h>
#include "pdistrib.h"

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
