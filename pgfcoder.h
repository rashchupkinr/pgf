/*
 * DFCencoder.h
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */

#ifndef DFCCODER_H_
#define DFCCODER_H_

#include "yuvimage.h"
#include "pdistrib.h"
#include "predictor.h"
#include "bitarr.h"

typedef struct _PDistribWeight {
	PDistrib pd;
	float weight;
} PDistribWeight;

class PGFCoder {
public:
	PGFCoder();
	virtual ~PGFCoder();
	void setYUVImage(YUVImage *_yuvimage)		{	yuvimage = _yuvimage;	}
	YUVImage *getYUVImage()						{	return yuvimage;		}
	bitarr *getCodes()							{	return codes;			}
	void setCodes(bitarr *_codes)				{	codes = _codes;			}
	int encode(int trim);
	int encode_plane(int pnum, int trim);
	int decode();
	int decode_plane(int pnum);
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
