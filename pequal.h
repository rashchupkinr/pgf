#ifndef __PEQUAL
#define __PEQUAL

#include "predictor.h"

class PEqual : public Predictor
{
public:
	PEqual(YUVImage *_img, int _dir, int _plane = -1): Predictor(_img, _dir, _plane)	{}
    void predict(int x, int y, PDistrib *pd);
private:
};

#endif
