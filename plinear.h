#ifndef __PLINEAR
#define __PLINEAR

#include "predictor.h"

class PLinear : public Predictor
{
public:
	PLinear(Image *_img, int _dir): Predictor(_img, _dir)	{}
	PDistrib predict(int x, int y);
private:
};

#endif
