#ifndef _CMSKETCH_H
#define _CMSKETCH_H

#include "params.h"
#include "BOBHash32.h"
#include "SPA.h"
#include <string.h>
#include <stdlib.h>

using namespace std;


class CMSketch: public SPA
{
private:
	int *CM;
	int w, d;
	int ww;
	BOBHash32 ** bobhash;

public:
	CMSketch(double M, int _d);
	~CMSketch();
	virtual void Insert(pkt pkt_t, int f);
	int Query(pkt pkt_t);

};

CMSketch::CMSketch(double M, int _d)
{
	d = _d;
	w = M * 1024 * 1024 * 8.0 / 32;
	ww = w * 1.0 / d;

	CM = new int[w];
	memset(CM, 0, sizeof(int) * w);

	bobhash = new BOBHash32 *[d];
	for(int i = 0; i < d; i++)
		bobhash[i] = new BOBHash32(i + 750);	

}
CMSketch::~CMSketch()
{
	delete []CM;
	for(int i = 0; i < d; i++)
		delete bobhash[i];
	delete[] bobhash;
}
void CMSketch::Insert(pkt pkt_t, int f)
{
	for(int i = 0; i < d; i++)
	{
		CM[(bobhash[i]->run((const char *)&pkt_t, PKT_LEN)) % ww + i * ww] += f;
	}
	return;
}
int CMSketch::Query(pkt pkt_t)
{
	int value;
	int V = 1 << 30;
	for(int i = 0; i < d; i++)
	{
		value = CM[(bobhash[i]->run((const char *)&pkt_t, PKT_LEN)) % ww + i * ww];

		V = V < value ? V : value;
	}
	return V;
}

#endif //_CMSKETCH_H