/*
 * main.cpp
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "logging.h"
#include "pgfpcoder.h"
#include "pgfcoder.h"
#include "yuvimage.h"
#include "bitarr.h"
#include "hcoder.h"
#include "arcoder.h"
#include "predictor.h"
#include "pdistribn.h"
using namespace std;

bool Encode;
int TestH = 0;
char *input_name=0, *output_name=0, *ref_name=0, *log_name=0;
int width = 0, height = 0;
YUV_FORMAT format = YUV444;

int process_arg(int argc, char **argv)
{
	for (;;)
	{
		int c;

        c = getopt(argc, argv, "ei:o:l:r:w:h:f:d:t:");
		if (c==EOF) break;
		switch (c)
		{
        case 'e':
        	Encode = true;
        	break;
		case 'i':
			input_name = optarg;
    		break;
		case 'o':
			output_name = optarg;
			break;
		case 'l':
			log_name = optarg;
			break;
		case 'r':
			ref_name = optarg;
			break;
        case 'f':
        	switch (atoi(optarg)) {
        	case 4:
        		format = YUV444;
        		break;
        	case 2:
        		format = YUV422;
        		break;
        	case 1:
        		format = YUV411;
        		break;
        	default:
        		dlog(LOG_FATAL, "Wrong format(not 1,2 or 4.\n");
        		exit(0);
        	}
            break;
        case 'w':
        	width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
		case 'd':
			log_priority(atoi(optarg));
			break;
        case 't':
			TestH = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Unknown option: %s\n", argv[optind-1]);
			return 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	log_priority(LOG_NORMAL);
	if (process_arg(argc, argv))
        exit(0);
	log_init(log_name);
	if ((!input_name || !output_name) && !TestH) {
		dlog(LOG_FATAL, "No input and output files specified.\n");
		exit(0);
	}
	if ((!width || !height) && !TestH) {
		dlog(LOG_FATAL, "No width or height specified.\n");
		exit(0);
	}
/*
PDistribN pd;
PDistribN::initUniformPDN(255);
pd.setUniformPD(255);
dlog(LOG_ALL,pd.print().c_str());
dlog(LOG_ALL,"\n");
PDistribN pd2;
pd2.setNullPD();
pd2.addSpikeEllipse(8,4,3);
pd = pd + pd2;
dlog(LOG_ALL,pd.print().c_str());
pd.normalize();
return 0;
*/
	if (TestH) {
		if (TestH == 2) {
			arcoder ac;
			ac.test();
		} else if (TestH == 1) {
			hcoder hc;
			hc.test();
		}
		return 0;
	}
	YUVImage *inp = new YUVImage(format, width, height);
	FILE *fin = fopen(input_name, "r");
//	YUVImage *inp = new YUVImage(YUV422, 16, 16);
//	FILE *fin = fopen("p1.yuv", "r");
    PGFPCoder coder;
	dlog(LOG_FATAL, "encode=%d w=%d h=%d format=%d i=%s o=%s\n",Encode, width, height, format, input_name, output_name);
	if (Encode) {
		inp->read(fin);
		coder.setYUVImage(inp);
		coder.encode(0);
/*	if (coder.getPredImage())
		dlog(LOG_FATAL,"codec diff =%f\n",inp->diff(coder.getPredImage()));
	coder.encode(2);
*/
		FILE *fout = fopen(output_name, "w");
		if (coder.getCodes()) {
			coder.getCodes()->write(fout);
//			for (int i=0;i<coder.getCodes()->size();i++)
//				dlog(LOG_VERBOSE, "%d",coder.getCodes()->get(i));
		}

		if (ref_name) {
			YUVImage *inp2 = new YUVImage(YUV444, width, height);
			FILE *fin2 = fopen(ref_name, "r");
			inp2->read(fin2);
			dlog(LOG_FATAL,"diff =%f\n",inp->diff(inp2));
		}
	} else {
		FILE *fout = fopen(output_name, "w");
		bitarr codes;
		codes.read(fin);
		coder.setCodes(&codes);
		YUVImage *yuv = new YUVImage(YUV444, width, height);
		coder.setYUVImage(yuv);
		coder.decode(fout);
		YUVImage *outp = coder.getYUVImage();
//		outp->write(fout);
	}
}



