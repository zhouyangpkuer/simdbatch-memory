#ifndef _MRAC_H
#define _MRAC_H

#include "params.h"
#include "BOBHash32.h"
#include "SPA.h"
#include <string.h>
#include <stdlib.h>

using namespace std;


class MRAC: public SPA
{
private:
	int *MR;
	int m, r;
	int M;
	int *fence;
	BOBHash32 *bobhash;

public:
	MRAC(double memory, int _r);
	~MRAC();
	virtual void Insert(pkt pkt_t, int f);
	int search(int hash_value);
};

MRAC::MRAC(double memory, int _r)
{
	r = _r;
	m = memory * 1024 * 1024 * 8.0 / (32 * (r + 1));
	M = m << r;

	MR = new int[m * (r + 1)]; // transforming m << r memory to m * r;
	memset(MR, 0, sizeof(int) * m * (r + 1));

	fence = new int[r + 2];
	int temp = M;
	int fence_value = 0;
	for(int i = 0; i < r + 1; i++)
	{
		fence[i] = fence_value;
		// printf("%d->%d\n", i, fence[i]);

		temp >>= 1;
		fence_value += temp;
	}
	fence[r + 1] = M;
	// printf("%d->%d\n", r + 1, fence[r + 1]);


	bobhash = new BOBHash32(750);
}

MRAC::~MRAC()
{
	delete []MR;
	delete []fence;
	delete bobhash;
}
int MRAC::search(int ind)
{
	for(int i = 0; i < r + 1; i++)
	{
		if(fence[i] <= ind && ind < fence[i + 1])
		{
			return i;
		}
	}
}
void MRAC::Insert(pkt pkt_t, int f)
{
	int ind = (bobhash->run((const char *)&pkt_t, PKT_LEN)) % M;
	int index = search(ind);

	MR[index * m + ind % m] += f;
	return;
}

#endif //_MRAC_H