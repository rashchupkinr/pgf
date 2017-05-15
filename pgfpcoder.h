/*
 * DFCencoder.h
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */

#ifndef PGFPCODER_H_
#define PGFPCODER_H_

#include <stdio.h>
#include "yuvimage.h"
#include "pdistrib.h"
#include "predictor.h"
#include "bitarr.h"


class PGFPCoder {
public:
	PGFPCoder();
	virtual ~PGFPCoder();
	void setYUVImage(YUVImage *_yuvimage)		{	yuvimage = _yuvimage;	}
	YUVImage *getYUVImage()						{	return yuvimage;		}
	bitarr *getCodes()							{	return codes;			}
	void setCodes(bitarr *_codes)				{	codes = _codes;			}
	int encode(int trim);
	int encode_plane(int pnum, int trim);
	int encode_plane_PDs(int pnum);
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
	PDistribWeight **pds[PREDICTOR_NUM];
	bitarr *codes;
	int decode_pos;
};

#endif /* DFCENCODER_H_ */
