/*
 * bits.h
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */

#ifndef BITS_H_
#define BITS_H_

#include <vector>
#include <stdio.h>
#include <iostream>
using namespace std;

class bitarr {
public:
	bitarr();
	virtual ~bitarr();
	void clear();
	bool operator==(const bitarr& bitarr2);
	operator uint64_t() const;
	void append(bool b);
	void append(uint64_t v, int n);
	void append(bitarr bar);
	bool get(size_t pos) const;
	void set(size_t pos, bool value);
	void cut(size_t pos);
    size_t	size()	const	{	return (array.size()-1)*64+cur_pos;	}
	int write(FILE *f);
	friend std::ostream& operator<<(std::ostream& os, const bitarr &b);
	int read(FILE *f);
	string print() const;
private:
    vector<uint64_t> array;
    uint64_t cur_pos;
};

#endif /* BITS_H_ */
