#ifndef _TWOLEVEL_H
#define _TWOLEVEL_H

#include "params.h"
#include "BOBHash32.h"
#include "SPA.h"
#include <string.h>
#include <stdlib.h>

using namespace std;


typedef struct _bitmap
{
	uint cell[3];
}bitmap;

class TwoLevel: public SPA
{
private:
	bitmap *CM;
	bitmap *Rev; // 4096 * 4 with 96-bit bitmap;
	int w, d;
	int ww;

	int d_rev;
	BOBHash32 ** bobhash;

public:
	TwoLevel(double M, int _d);
	~TwoLevel();
	virtual void Insert(pkt pkt_t, int f);
	uint RevHash(pkt pkt_t, int k);
	void SetBit(bitmap *bitmaps, int bitmap_index, pkt pkt_t);
};
// the memory and hash function number of CM sketch.
TwoLevel::TwoLevel(double M, int _d)
{
	d = _d;
	d_rev = 2; // the number of array in rev;


	w = M * 1024 * 1024 * 8.0 / sizeof(bitmap);
	ww = w * 1.0 / d;

	CM = new bitmap[w];
	memset(CM, 0, sizeof(bitmap) * w);

	Rev = new bitmap[4096 * d_rev];
	memset(Rev, 0, sizeof(bitmap) * 4096 * d_rev);

	bobhash = new BOBHash32 *[d + 4 * d_rev + 1];
	for(int i = 0; i < d + 4 * d_rev + 1; i++)
		bobhash[i] = new BOBHash32(i + 750);

}
TwoLevel::~TwoLevel()
{
	delete []CM;
	delete []Rev;
	for(int i = 0; i < d + 4 * d_rev + 1; i++)
		delete bobhash[i];
	delete[] bobhash;
}
void TwoLevel::Insert(pkt pkt_t, int f)
{
	for(int i = 0; i < d; i++)
	{
		SetBit(CM, (bobhash[i + 4 * d_rev]->run((const char *)&pkt_t, 4)) % ww + i * ww, pkt_t);
	}
	for(int i = 0; i < d_rev; i++)
	{
		SetBit(Rev, RevHash(pkt_t, i) + i * 4096, pkt_t);
	}
	return;
}
void TwoLevel::SetBit(bitmap *bitmaps, int bitmap_index, pkt pkt_t)
{
	uint index_in_bitmap = bobhash[d + 4 * d_rev]->run((const char *)&(pkt_t.dip), 4) % 96;
	int a = index_in_bitmap / 32;
	int b = index_in_bitmap % 32;

	bitmaps[bitmap_index].cell[a] |= 1 << b;
}

uint TwoLevel::RevHash(pkt pkt_t, int k)
{
	uint hashvalue = 0;
	uint partvalue = 0;

	pkt_t.sip = (uint)(((ull)pkt_t.sip * (ull)100000007) & 0xFFFFFFFF); 
	unsigned char *cursor = (unsigned char *)&pkt_t;
	for(int i = 0; i < 4; i++)
	{
		partvalue = (bobhash[k * 4 + i]->run((const char *)(cursor + i), 1)) & 0x7; // 3-bit mask! for 2^12-bit row
		partvalue <<= (i * 3);
		hashvalue |= partvalue;
	}

	return hashvalue;

}
#endif //_TWOLEVEL_H