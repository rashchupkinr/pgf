#include "predictor.h"

class PMed : public Predictor
{
public:
	PMed(Image *_img, int _dir): Predictor(_img, _dir)	{}
	PDistrib predict(int x, int y);
private:
};
