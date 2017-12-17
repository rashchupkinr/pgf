#ifndef __PEQUAL
#define __PEQUAL

#include "predictor.h"

class PEqual : public Predictor
{
public:
	PEqual(YUVImage *_img, int _dir, int _plane = -1): Predictor(_img, _dir, _plane)	{}
    int predict(int x, int y, PDistrib *pd, double priority, int rad);
private:
};

#endif
