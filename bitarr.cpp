/*
 * bits.cpp
 *
 *  Created on: 19.12.2014
 *      Author: roman
 */

#include "bitarr.h"
#include <iostream>

bool bitarr::operator==(const bitarr& bitarr2)
{
	if (size() != bitarr2.size())
		return false;
	for (int i=0; i<array.size(); i++)
		if (array[i] != bitarr2.array[i])
			return false;
	return true;
}

bitarr::operator uint64_t() const
{
	if (size() > 64)
		return 0;
	uint64_t v = 0;
	for (int i=0; i<size(); i++) {
		v <<= 1;
		if (get(i))
			v |= 1;
	}
	return v;
}

string bitarr::print() const
{
	string str = "{";
	for (int i=0; i<size(); i++)
		str += std::to_string(get(i));
	str += "}";
	return str;
}

std::ostream& operator<<(std::ostream& os, const bitarr &b)
{
	return os << b.print();
}

void bitarr::append(bool b)
{
	unsigned char c;
	if (array.size() == 0)
		c = 0;
	else
		c = array[array.size() - 1];
	if (cur_pos < 8) {
		c |= b << cur_pos;
		cur_pos++;
		array[array.size() - 1] = c;
	} else {
		c = b;
		array.push_back(c);
		cur_pos = 1;
	}
}

void bitarr::append(uint64_t v, int n)
{
	for (int i=n-1; i>=0; i--)
		append(v & (1ULL << i));
}

void bitarr::append(bitarr bar)
{
	for (int i=0; i<bar.size(); i++)
		append(bar.get(i));
}

void bitarr::set(size_t pos, bool value)
{
	size_t p = pos / 8;
	if (p > array.size())
		return;
	pos -= p * 8;
	array[p] &= ~(1<<pos);
	array[p] |= value << pos;
}

bool bitarr::get(size_t pos) const
{
	size_t p = pos / 8;
	if (p > array.size())
		return false;
	pos -= p * 8;
	return (array[p] & (1 << pos)) >> pos;
}

void bitarr::cut(size_t pos)
{
	if (pos > size())
		return;
	bitarr tmp;
	for (int i=0; i<pos; i++)
		tmp.append(get(i));
	for (int i=pos+1; i<size(); i++)
		tmp.append(get(i));
	array = tmp.array;
	cur_pos = tmp.cur_pos;
}

int bitarr::write(FILE *f)
{
	int offset = 0;
	if (fseek(f, offset, SEEK_SET))
		return -1;
	fwrite(&cur_pos, sizeof(char), 1, f);
	for (int i=0; i<array.size();i++) {
		unsigned char v = array[i];
		if (fwrite(&v, sizeof(char), 1, f) != sizeof(char))
			return -1;
	}
	return 0;
}

int bitarr::read(FILE *f)
{
	int offset = 0;
	if (fseek(f, offset, SEEK_SET))
		return -1;
	fread(&cur_pos, sizeof(char), 1, f);
	unsigned char v;
	while (fread(&v, sizeof(char), 1, f) == sizeof(char))
		array.push_back(v);
	return 0;
}

void bitarr::clear()
{
	array.clear();
	cur_pos = 8;
}

bitarr::bitarr() {
	cur_pos = 8;
}

bitarr::~bitarr() {
}

