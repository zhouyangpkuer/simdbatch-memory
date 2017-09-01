#ifndef _FMSKETCH_H
#define _FMSKETCH_H
#include "BOBHash32.h"
#include <cstring>
#include "SPA.h"
#include "params.h"
#include <math.h>
#include <bmiintrin.h>


class FMSketch: public SPA
{
private:
	int d, w;
	int * counter;
	BOBHash32 * bobhash[MAX_HASH_NUM];
public:
	FMSketch(int _d, int _w)
	{
		d = _d;
		w = _w;
		counter = new int[d];
		memset(counter, 0, sizeof(int) * d);

		for(int i = 0; i < d; i++)
		{
			bobhash[i] = new BOBHash32(i);
		}
	}
	~FMSketch()
	{
		delete []counter;
		for(int i = 0; i < d; i++)
			delete bobhash[i];

	}
	
	//return how many 0 bit in the lower hashvalue;
	int MyHash(int k, pkt pkt_t)
	{
		uint32_t HashValue = bobhash[k]->run((const char *)&pkt_t, PKT_LEN);
        return (HashValue == 0 ? w - 1 : _tzcnt_u32(HashValue));
/*
		for(int i = 0; i < w - 1; i++)
		{
			if(((HashValue >> i) & 1) == 1)
			{
				printf("i = %d\n", i);
				return i;
			}
		}
*/

		//commonly it will not reach here!
		// return w - 1;
	}
	void Insert(pkt pkt_t, int f)
	{
		int hashNum;
		for(int i = 0; i < d; i++)
		{
			hashNum = MyHash(i, pkt_t);
			counter[i] |= (1 << hashNum);
		}
	}
	int Query()
	{
		double avelow = 0, avehigh = 0;
		for(int x = 0; x < d; x++)
		{
			int i, low, high;
			for(i = 0; i < w; i++)
			{
				if(((counter[x] >> i) & 1) == 0)
				{
					low = i;
					break;
				}
			}
			for(i = w - 1; i >= 0; i--)
			{
				if(((counter[x] >> i) & 1) == 1)
				{
					high = i + 1;
					break;
				}
			}
			
			// low, high

			// printf("low = %d, high = %d\n", low, high);
			avelow += low;
			avehigh += high;
		}
		// return (int)pow(2.0, avehigh / d);

		return (int)(pow(2.0, (avelow / d) / 1) * 1.2928);
	}

};





#endif