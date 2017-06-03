/*
 * DFCencoder.h
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */

#ifndef PGFCODER_H_
#define PGFCODER_H_

#include <stdio.h>
#include "yuvimage.h"
#include "pdistrib.h"
#include "predictor.h"
#include "bitarr.h"
#include "c3x.h"
#include "pancoder.h"
#include "pandistrib.h"


class PGFCoder {
public:
	PGFCoder();
	virtual ~PGFCoder();
	void setYUVImage(YUVImage *_yuvimage)
	{
		if (_yuvimage->getYUVFormat() != YUV444)
			return;
		yuvimage = _yuvimage;
	}
	YUVImage *getYUVImage()						{	return yuvimage;		}
	bitarr *getCodes()							{	return codes;			}
	void setCodes(bitarr *_codes)				{	codes = _codes;			}
	int encode(int trim);
	int encode_PDs();
	int decode(FILE *fout = 0);
	int decode_plane(int pnum,FILE *fout = 0);
	YUVImage *getCurImage()						{	return cur_image;		}
	void remove_edges();
	void test_edge();
	void compute_stat();
private:
	YUVImage *yuvimage;
	YUVImage *cur_image;
	YUVImage *edge_image;
	YUVImage *pred_image;
	vector<edge> edges;
	bitarr *codes;
	int decode_pos;
};

#endif /* DFCENCODER_H_ */
