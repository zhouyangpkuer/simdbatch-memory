#ifndef _LC_H
#define _LC_H

#include "params.h"
#include "BOBHash32.h"
#include "SPA.h"
#include <string.h>
#include <stdlib.h>

using namespace std;


class LC: public SPA
{
private:
	int *counters;
	int w, d;
	int ww;
	BOBHash32 ** bobhash;

public:
	LC(double M, int _d);
	~LC();
	virtual void Insert(pkt pkt_t, int f);

};

LC::LC(double M, int _d)
{
	d = _d;
	w = M * 1024 * 1024 * 8.0;// total bit number
	ww = w * 1.0 / d;// bit number per array

	counters = new int[w / 32];
	memset(counters, 0, sizeof(int) * w / 32);

	bobhash = new BOBHash32 *[d];
	for(int i = 0; i < d; i++)
		bobhash[i] = new BOBHash32(i + 750);	

}
LC::~LC()
{
	delete []counters;
	for(int i = 0; i < d; i++)
		delete bobhash[i];
	delete[] bobhash;
}
void LC::Insert(pkt pkt_t, int f)
{
	int index, a, b;
	for(int i = 0; i < d; i++)
	{
		index = (bobhash[i]->run((const char *)&pkt_t, PKT_LEN)) % ww;
		a = index / 32;
		b = index % 32;
		counters[i * ww / 32 + a] |= (1 << b);
	}
	return;
}

#endif //_LC_H