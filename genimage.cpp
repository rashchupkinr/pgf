/*
 * image.cpp
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */

#include <math.h>
#include "genimage.h"

CIF_FORMAT_SIZE GenImage::CIF_FORMAT_SIZES[] = { { SF_SQCIF, 128, 96 }, { SF_QCIF,
		176, 144 }, { SF_CIF, 352, 288 }, { SF_4CIF, 704, 576 }, { SF_16CIF,
		1408, 1152 }, { SF_NONE, 0, 0 } };

#include "logging.h"
double GenImage::diff(GenImage *img) {
	if (getPlaneNum() != img->getPlaneNum())
		return 0;
	for (int k = 0; k < img->getPlaneNum(); k++)
		if (getWidth(k) != img->getWidth(k)
				|| getHeight(k) != img->getHeight(k))
			return 0;
	double d = 0;
	for (int k = 0; k < img->getPlaneNum(); k++) {
		double dp = 0;
		for (int i = 0; i < img->getWidth(k); i++)
			for (int j = 0; j < img->getHeight(k); j++)
				dp += pow(img->getPlane(k)->get(i, j) - getPlane(k)->get(i, j), 2);
//			dp += (img->getPlane(k)->data[i] - getPlane(k)->data[i])
//					* (img->getPlane(k)->data[i] - getPlane(k)->data[i]);
		dp /= img->getWidth(k) * img->getHeight(k);
		d += dp;
		dlog(LOG_FATAL, "%f ", dp);
	}
	d /= getPlaneNum();
	return d;
}
