#ifndef __PEQUAL
#define __PEQUAL

#include "predictor.h"

class PEqual : public Predictor
{
public:
	PEqual(YUVImage *_img, int _dir, int _plane = -1): Predictor(_img, _dir, _plane)	{}
	PDistrib predict(int x, int y);
private:
};

#endif
