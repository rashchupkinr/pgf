#ifndef __PLINEAR
#define __PLINEAR

#include "predictor.h"

class PLinear : public Predictor
{
public:
	PLinear(YUVImage *_img, int _dir, int _plane = -1): Predictor(_img, _dir, _plane)	{}
    void predict(int x, int y, PDistrib *pd);
private:
};

#endif
