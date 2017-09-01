#ifndef _REVSKETCH_H
#define _REVSKETCH_H

#include "params.h"
#include "BOBHash32.h"
#include "SPA.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

// not standard implementation, just for simulate the inserting process!
class RevSketch: public SPA
{
private:
	int *Rev;
	int w, d;
	int ww;
	BOBHash32 ** bobhash;

public:
	RevSketch(double M, int _d);
	~RevSketch();
	virtual void Insert(pkt pkt_t, int f);
	uint RevHash(pkt pkt_t, int k);

};

// d is the number of arrays, not the number of hash functions;
// the latter is 4 * d or 6 * d or 7 * d;
// memory should be fixed on 1\2\4\8\16... MB;
// _d should be fixed on 4;
RevSketch::RevSketch(double M, int _d)
{
	d = _d;
	w = M * 1024 * 1024 * 8.0 / 32;
	ww = w * 1.0 / d;
	// printf("ww = %d\n", ww);

	Rev = new int[w];
	memset(Rev, 0, sizeof(int) * w);

	bobhash = new BOBHash32 *[8 * d];
	for(int i = 0; i < 8 * d; i++)
		bobhash[i] = new BOBHash32(i + 750);

}
RevSketch::~RevSketch()
{
	delete []Rev;
	for(int i = 0; i < d; i++)
		delete bobhash[i];
	delete[] bobhash;
}
void RevSketch::Insert(pkt pkt_t, int f)
{
	int index[MAX_HASH_NUM];
	for(int i = 0; i < d; i++)
	{
		Rev[RevHash(pkt_t, i) % ww + i * ww] += f;
	}
	return;
}
uint RevSketch::RevHash(pkt pkt_t, int k)
{
	uint hashvalue = 0;
	uint partvalue = 0;

#ifdef SIP_PKT
	pkt_t.sip = (uint)(((ull)pkt_t.sip * (ull)100000007) & 0xFFFFFFFF); 
	unsigned char *cursor = (unsigned char *)&pkt_t;
	for(int i = 0; i < 4; i++)
	{
		partvalue = (bobhash[k * 4 + i]->run((const char *)(cursor + i), 1)) & 0xFF;
		partvalue <<= (i * 8);
		hashvalue |= partvalue;
	}
#endif

#ifdef DIP_PKT
	pkt_t.dip = (uint)(((ull)pkt_t.dip * (ull)100000007) & 0xFFFFFFFF); 
	unsigned char *cursor = (unsigned char *)&pkt_t;
	for(int i = 0; i < 4; i++)
	{
		partvalue = (bobhash[k * 4 + i]->run((const char *)(cursor + i), 1)) & 0xFF;
		partvalue <<= (i * 8);
		hashvalue |= partvalue;
	}
#endif

#ifdef SIP_DIP_PKT
	pkt_t.sip = (uint)(((ull)pkt_t.sip * (ull)100000007) & 0xFFFFFFFF); 
	pkt_t.dip = (uint)(((ull)pkt_t.dip * (ull)100000007) & 0xFFFFFFFF); 
	unsigned char *cursor = (unsigned char *)&pkt_t;
	for(int i = 0; i < 4; i++)
	{
		partvalue = (bobhash[k * 4 + i]->run((const char *)(cursor + i * 2), 2)) & 0xFF;
		partvalue <<= (i * 8);
		hashvalue |= partvalue;
	}
#endif

#ifdef FOUR_TUPLE_PKT
	pkt_t.sip = (uint)(((ull)pkt_t.sip * (ull)100000007) & 0xFFFFFFFF); 
	pkt_t.dip = (uint)(((ull)pkt_t.dip * (ull)100000007) & 0xFFFFFFFF); 
	unsigned char *cursor = (unsigned char *)&pkt_t;
	for(int i = 0; i < 6; i++)
	{
		partvalue = (bobhash[k * 6 + i]->run((const char *)(cursor + i * 2), 2)) & 0xFF;
		partvalue <<= (i * 6);//36 bits
		hashvalue |= partvalue;
	}
#endif

#ifdef FIVE_TUPLE_PKT
	pkt_t.sip = (uint)(((ull)pkt_t.sip * (ull)100000007) & 0xFFFFFFFF); 
	pkt_t.dip = (uint)(((ull)pkt_t.dip * (ull)100000007) & 0xFFFFFFFF); 
	unsigned char *cursor = (unsigned char *)&pkt_t;
	for(int i = 0; i < 7; i++)
	{
		partvalue = (bobhash[k * 7 + i]->run((const char *)(cursor + i * 2), 2)) & 0xFF;
		partvalue <<= (i * 5);//35 bits
		hashvalue |= partvalue;
	}
#endif

	return hashvalue;

}
#endif //_REVSKETCH_H