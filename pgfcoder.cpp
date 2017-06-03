/*
 * DFCencoder.cpp
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */

#include <math.h>
#include <stdlib.h>
#include "pgfcoder.h"
#include "image.h"
#include "logging.h"
#include "hcoder.h"
#include "pmed.h"
#include "pequal.h"
#include "plinear.h"
#include "psim.h"
#include "c3x.h"
using namespace std;

extern void signal_stats(int signal);


void PGFCoder::remove_edges()
{
	edge_image = new YUVImage(cur_image);
	for (int k=0; k<yuvimage->getPlaneNum(); k++) {
		Image *img = cur_image->getPlane(k);
		Image *edge_img = edge_image->getPlane(k);
		edges = img->find_edges();
		for (int i=0; i<edges.size(); i++) {
			edge e = edges[i];
			int v = img->get(e.x, e.y);
			edge_img->set(e.x, e.y, v);
			img->set(e.x, e.y, EDGE);
		}
	}
}

int PGFCoder::encode(int trim)
{

	PAnDistrib pad(PNUM, PMIN_RADIUS, PMAX_RADIUS);
	pad.AddArea(c3x(10, 10, 10),4);
	pad.AddArea(c3x(16, 16, 16),4);
	dlog(LOG_FATAL, "pad: %s\n", pad.print().c_str());
	exit(0);
    PDistribA::initUniformPD(yuvimage->getPlane(0)->getMaxValue());
	dlog(LOG_NORMAL, "\nencode\n");
	PMed pMed = PMed(yuvimage, PREDICTOR_DIR_NUM);
	PEqual pEqual[PREDICTOR_DIR_NUM] = {	PEqual(yuvimage, PREDICTOR_DIR_L),
											PEqual(yuvimage, PREDICTOR_DIR_LU),
											PEqual(yuvimage, PREDICTOR_DIR_U),
											PEqual(yuvimage, PREDICTOR_DIR_RU)};
	PLinear pLinear[PREDICTOR_DIR_NUM] = {	PLinear(yuvimage, PREDICTOR_DIR_L),
											PLinear(yuvimage, PREDICTOR_DIR_LU),
											PLinear(yuvimage, PREDICTOR_DIR_U),
											PLinear(yuvimage, PREDICTOR_DIR_RU)};
	PSim pSim = PSim(yuvimage);
//	hcoder hc;
/*
    for (int y=0; y<yuvimage->getHeight(0); y++) {
		float bits_row = 0;
		for (int x=0; x<yuvimage->getWidth(0); x++) {
			dlog(LOG_ALL, "e[%d,%d]=%s\n",  y, x, yuvimage->get(x, y).print().c_str());
            PDistribA pd;
            pd.setUniformPD(yuvimage->getMaxValue());
			{
				PDistrib pdMed = pMed.predict(x, y);
//				pds[PREDICTOR_MED][pnum][y*img->getWidth()+x].pd = pdMed;
				pd = pd + pdMed;
				dlog(LOG_ALL,"pdm %s\n", pdMed.print().c_str());
			}
//if (0)
			for (int dir=0; dir<PREDICTOR_DIR_NUM; dir++) {
				PDistrib pdEqual = pEqual[dir].predict(x, y);
//				pds[PREDICTOR_EQUAL+dir][pnum][y*img->getWidth()+x].pd = pdEqual;
				pd = pd + pdEqual;
				dlog(LOG_ALL,"pde %s\n", pdEqual.print().c_str());
			}

			if (0)
			for (int dir=0; dir<PREDICTOR_DIR_NUM; dir++) {
				PDistrib pdLinear = pLinear[dir].predict(x, y);
//				pds[PREDICTOR_LINEAR+dir][pnum][y*img->getWidth()+x].pd = pdLinear;
				pd = pd + pdLinear;
				dlog(LOG_ALL,"pdl %s\n", pdLinear.print().c_str());
			}

//			if (0)
			{
				PDistrib pdSim = pSim.predict(x, y);
//				pds[PREDICTOR_MED][pnum][y*img->getWidth()+x].pd = pdMed;
				pd = pd + pdSim;
				dlog(LOG_ALL,"pds %s\n", pdSim.print().c_str());
			}
			yuvimage->setPD(x, y, pd);
*/
			/*
			pd.sort();
//			continue;
			pd.normalize();
			dlog(LOG_ALL,"pd %s\n", pd.print().c_str());
			if (hc.setPDistrib(pd, true) < 0) {
				hc.dump(LOG_FATAL);
				dlog(LOG_FATAL, "Encode error(hc.setPDistrib).\n");
//				dlog(LOG_FATAL,"pds %s\n", pdSim.print().c_str());
				dlog(LOG_FATAL,"[%d, %d]: pd %s", pd.print().c_str(), y, x);
				return -1;
			}
//hc.dump();
			bitarr code = hc.encode(img->get(x, y));
			codes->append(code);
			bits_row += code.size();

			dlog(LOG_ALL,"coded[%d,%d] %d_%s|%d|\n", y, x, img->get(x,y), code.print().c_str(),codes->size());
		}
		dlog(LOG_NORMAL, "r%d ", y);
	}
*/	encode_PDs();
	return 0;
}

int PGFCoder::encode_PDs()
{
	dlog(LOG_NORMAL, "\nencode_plane_PDs\n");
	hcoder hc;
	for (int y=0; y<yuvimage->getHeight(0); y++) {
		float bits_row = 0;
		for (int x=0; x<yuvimage->getWidth(0); x++) {
			dlog(LOG_ALL, "w[%d,%d]\n", y, x);
            PDistribA pd = yuvimage->getPD(x, y);
			pd.sort();
		//			continue;
			pd.normalize();
			dlog(LOG_ALL,"pd %s\n", pd.print().c_str());
            if (hc.setPDistrib(&pd, true) < 0) {
				hc.dump(LOG_FATAL);
				dlog(LOG_FATAL, "Encode error(hc.setPDistrib).\n");
		//				dlog(LOG_FATAL,"pds %s\n", pdSim.print().c_str());
				dlog(LOG_FATAL,"[%d, %d]: pd %s", pd.print().c_str(), y, x);
				return -1;
			}
		//hc.dump();
			bitarr code = hc.encode(yuvimage->get(x, y));
			codes->append(code);
			bits_row += code.size();

			dlog(LOG_ALL,"coded[%d,%d] %s_%s|%d|\n", y, x, yuvimage->get(x,y).print().c_str(), code.print().c_str(),codes->size());
			//		dlog(LOG_NORMAL, "r%d{%.2f} ", y, bits_row/img->getWidth());
		}
		dlog(LOG_NORMAL, "R%d{%.2f} ", y, bits_row/yuvimage->getWidth(0));
	}
}

int PGFCoder::decode(FILE *fout)
{
    PDistribA::initUniformPD(yuvimage->getPlane(0)->getMaxValue());
	decode_pos = 0;
	for (int i=0; i<yuvimage->getPlaneNum(); i++) {
		int ret = decode_plane(i, fout);
		if (ret < 0)
			return ret;
	}
	return 0;
}

int PGFCoder::decode_plane(int pnum, FILE *fout)
{
	Image *img = yuvimage->getPlane(pnum);
	PMed pMed = PMed(yuvimage, PREDICTOR_DIR_NUM);
	PEqual pEqual[PREDICTOR_DIR_NUM] = {	PEqual(yuvimage, PREDICTOR_DIR_L),
											PEqual(yuvimage, PREDICTOR_DIR_LU),
											PEqual(yuvimage, PREDICTOR_DIR_U),
											PEqual(yuvimage, PREDICTOR_DIR_RU)};
	PLinear pLinear[PREDICTOR_DIR_NUM] = {	PLinear(yuvimage, PREDICTOR_DIR_L),
											PLinear(yuvimage, PREDICTOR_DIR_LU),
											PLinear(yuvimage, PREDICTOR_DIR_U),
											PLinear(yuvimage, PREDICTOR_DIR_RU)};
	PSim pSim = PSim(yuvimage);
	hcoder hc;
    /*
	for (int y=0; y<img->getHeight(); y++) {
		for (int x=0; x<img->getWidth(); x++) {
			dlog(LOG_ALL, "%d d[%d,%d]\n", pnum, y, x);
            PDistrib pd;
            pd.setUniformPD(yuvimage->getMaxValue());

			{
				PDistrib pdMed = pMed.predict(x, y);
//				pds[PREDICTOR_MED][pnum][y*img->getWidth()+x].pd = pdMed;
				pd = pd + pdMed;
				dlog(LOG_ALL,"pdm %s\n", pdMed.print().c_str());
			}
//3			if (0)
			for (int dir=0; dir<PREDICTOR_DIR_NUM; dir++) {
				PDistrib pdEqual = pEqual[dir].predict(x, y);
//				pds[PREDICTOR_EQUAL+dir][pnum][y*img->getWidth()+x].pd = pdEqual;
				pd = pd + pdEqual;
				dlog(LOG_ALL,"pde %s\n", pdEqual.print().c_str());
			}

//			if (0)
			for (int dir=0; dir<PREDICTOR_DIR_NUM; dir++) {
				PDistrib pdLinear = pLinear[dir].predict(x, y);
//				pds[PREDICTOR_LINEAR+dir][pnum][y*img->getWidth()+x].pd = pdLinear;
				pd = pd + pdLinear;
				dlog(LOG_ALL,"pdl %s\n", pdLinear.print().c_str());
			}

			if (0)
			{
				PDistrib pdSim = pSim.predict(x, y);
//				pds[PREDICTOR_MED][pnum][y*img->getWidth()+x].pd = pdMed;
				pd = pd + pdSim;
				dlog(LOG_ALL,"pds %s\n", pdSim.print().c_str());
			}
			img->setPD(x, y, pd);

			pd.sort();
			pd.normalize();
			dlog(LOG_ALL,"pd %s\n", pd.print().c_str());
			if (hc.setPDistrib(pd, true) < 0) {
				hc.dump(LOG_FATAL);
				dlog(LOG_FATAL, "Decode error(hc.setPDistrib).\n");
//				dlog(LOG_FATAL,"pds %s\n", pdSim.print().c_str());
				dlog(LOG_FATAL,"[%d, %d]: pd %s", y , x, pd.print().c_str());
				return -1;
			}
			long v;
	//dlog(LOG_ERROR,"%s\n",pd.print().c_str());
			int len = hc.decode(codes, decode_pos, &v);
			if (len == 0) {
				dlog(LOG_FATAL, "Decode error(hc.decode).\n");
				hc.dump(LOG_FATAL);
				dlog(LOG_FATAL, "[%d, %d] decoding: ", y, x);
				for (int i=0; i<16;i++)
					dlog(LOG_FATAL, "%d", codes->get(decode_pos+i));
				dlog(LOG_FATAL, "\n");
				return -1;
			}
			dlog(LOG_ALL,"decoded[%d,%d] %d=%x{", y, x, v, v);
			for (int i=0;i<len;i++)
				dlog(LOG_ALL,"%d", codes->get(decode_pos+i));
			dlog(LOG_ALL,"}|%d|\n",decode_pos+len);
//dlog(LOG_ERROR,"%x ", v);
			decode_pos += len;

			img->set(x, y, v);
		}
		dlog(LOG_NORMAL, "r%d ", y);
		yuvimage->writeRow(fout, pnum, y);
	}
    */
	return 0;
}

void PGFCoder::test_edge()
{
	std::vector<edge> edges;
	cur_image = new YUVImage(yuvimage);
	cur_image->copy(yuvimage);
	Image *img = cur_image->getPlane(PLANE_LUM);
	dlog_block(LOG_NORMAL, img, 0, 0, 8, 8);
	remove_edges();
	dlog_block(LOG_NORMAL, img, 0, 0, 8, 8);
}

PGFCoder::PGFCoder()
: yuvimage(0),
  cur_image(0),
  edge_image(0),
  pred_image(0)
//  coef(0)
{
	codes = new bitarr();
}

PGFCoder::~PGFCoder()
{
/*	if (coef && yuvimage) {
		delete coef;
	}*/
	if (pred_image)
		delete pred_image;
	if (edge_image)
		delete edge_image;
	if (cur_image)
		delete cur_image;
	if (yuvimage)
		delete yuvimage;
}

