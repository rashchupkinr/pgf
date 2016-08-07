#ifndef __PEQUAL
#define __PEQUAL

#include "predictor.h"

class PEqual : public Predictor
{
public:
	PEqual(Image *_img, int _dir): Predictor(_img, _dir)	{}
	PDistrib predict(int x, int y);
private:
};

#endif
