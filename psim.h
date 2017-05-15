#ifndef __PSIM
#define __PSIM

#include "predictor.h"

class PSim : public Predictor
{
public:
	PSim(YUVImage *_img, int _plane = -1): Predictor(_img, 0, _plane)	{}
	PDistrib predict(int x, int y);
private:
};

#endif
