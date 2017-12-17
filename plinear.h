#ifndef __PLINEAR
#define __PLINEAR

#include "predictor.h"

class PLinear : public Predictor
{
public:
	PLinear(YUVImage *_img, int _dir, int _plane = -1): Predictor(_img, _dir, _plane)	{}
    int predict(int x, int y, PDistrib *pd, double priority, int rad);
private:
};

#endif
