#ifndef __PSIM
#define __PSIM

#include "predictor.h"

class PSim : public Predictor
{
public:
	PSim(Image *_img): Predictor(_img, 0)	{}
	PDistrib predict(int x, int y);
private:
};

#endif
