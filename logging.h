/*
 * TranslateServer.h
 *
 *  Created on: 25.04.2014
 *      Author: Raschupkin Roman
 */

#ifndef LOGGING_H_
#define LOGGING_H_
#include "image.h"

#define LOG_FILENAME	"pgf.log"
#define LOG_ALL	5
#define LOG_VERBOSE	4
#define	LOG_NORMAL	3
#define	LOG_WARNING	2
#define LOG_ERROR	1
#define LOG_FATAL	0

void dlog_block(int level, Image *img, int x, int y, int bw, int bh);
void dlog_coef(int level, unsigned char ***coef, int steps, int x, int y);
void dlog(int priority, const char *msg, ...);
bool is_dlog(int priority);
int log_init(char *log_name);
void log_finit();
void log_priority(int p);

#endif /* LOGGING_H_ */
