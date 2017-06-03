/*
 * logging.cpp
 *
 *  Created on: 25.04.2014
 *      Author: Raschupkin Roman
 */

#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "logging.h"
#include "image.h"

FILE *LogFile=0;
int _DLOG_cur = LOG_FATAL;
int LogPriority = LOG_ALL;

bool is_dlog(int priority)
{
	if (priority > LogPriority)
		return false;
	return true;
}

void dlog_block(int level, Image *img, int x, int y, int bw, int bh)
{
	dlog(level, "Block %d-%d[%d][%d]:\n", x, y, bw, bh);
	for (int i=0; i<bh; i++) {
		for (int j=0; j<bw; j++) {
			dlog(level, "%3d ", img->get(x+j, y+i));
		}
		dlog(level, "\n");
	}
	dlog(level, "\n");
}

void dlog_coef(int level, unsigned char ***coef, int steps, int x, int y)
{
	dlog(level, "Coef %d-%d:\n", x, y);
	int bw=1, bh=1;
	for (int s=0; s<steps; s++) {
		dlog(level, "Step %d:\n", s);
		for (int i=0; i<bh; i++) {
			for (int j=0; j<bw; j++)
				dlog(level, "%3d ", coef[s][x+j][y+i]);
			dlog(level, "\n");
		}
		dlog(level, "\n");
		bw*=2;
		bh*=2;
	}
	dlog(level, "\n");
}

void dlog(int priority, const char *msg, ...)
{
    _DLOG_cur = priority;
	if (priority > LogPriority)
		return;
	time_t t = time(0);
	struct tm *time = localtime(&t);
	char tbuf[512];
//	strftime(tbuf, 512, "%D %H:%M:%S|\t", time);
	va_list args;
	if (priority <= LogPriority) {
		if (LogFile) {
//			fprintf(LogFile, (char *)tbuf);
			va_start(args, msg);
			vfprintf(LogFile, msg, args);
			va_end(args);
//			fprintf(LogFile, "\n");
			fflush(LogFile);
		}
//		fprintf(stdout, (char *)tbuf);
		va_start(args, msg);
		vfprintf(stdout, msg, args);
		va_end(args);
//		fprintf(stdout, "\n");
		fflush(stdout);
	}
}

int log_init(char *log_name)
{
	if (log_name)
		LogFile = fopen(log_name, "a");
	else
		LogFile = fopen(LOG_FILENAME, "a");
	if (!LogFile) {
		fprintf(stderr, "Error opening log file.\n");
		return -1;
	}
	return 0;
}

void log_finit()
{
	if (LogFile)
		fclose(LogFile);
}

void log_priority(int p)
{
	if (p < LOG_FATAL || p > LOG_ALL)
		return;
	LogPriority = p;
}



