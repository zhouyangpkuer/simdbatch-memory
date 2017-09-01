#ifndef _SC_H
#define _SC_H

#include "params.h"
#include "BOBHash32.h"
#include "SPA.h"
#include <string.h>
#include <x86intrin.h>
#include <bmiintrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

class SC
{
private:
	int bucket_num, counter_num;
	
 	pkt ** ori_pkt; // original pkt
	uint ** FP; // fingerprint
	int ** counter; // for counting aggregated frequence
	int *cur_pos;

#ifdef GRR
	int cur_kick;
#endif

#ifdef LRU
	uint time_now;
	uint ** myclock;
#endif

	SPA * spa;

public:
	SC(int _bucket_num, int _counter_num);

	~SC();

	void init_spa(SPA * _spa);

	void Insert(pkt pkt_t, int f);

	void Insert_SC_SPA(pkt pkt_t, int kick_f);

	void refresh();

};

SC::SC(int _bucket_num, int _counter_num)
{
	//init the buckets
	bucket_num = _bucket_num;
	counter_num = _counter_num;

	cur_pos = new int[bucket_num];
	memset(cur_pos, 0, sizeof(int) * bucket_num);

	ori_pkt = new pkt *[bucket_num];
	FP = new uint * [bucket_num];
	counter = new int * [bucket_num];
	for(int i = 0; i < bucket_num; i++)
	{
		ori_pkt[i] = new pkt[counter_num];
		memset(ori_pkt[i], 0, sizeof(pkt) * counter_num);
		FP[i] = new uint[counter_num];
		memset(FP[i], 0, sizeof(uint) * counter_num);
		counter[i] = new int[counter_num];
		memset(counter[i], 0, sizeof(int) * counter_num);
	}

	spa = NULL;


#ifdef GRR
	cur_kick = 0;
#endif

#ifdef LRU
	time_now = 0;
	myclock = new uint * [bucket_num];
	for(int i = 0; i < bucket_num; i++)
	{
		myclock[i] = new uint[counter_num];
		memset(myclock[i], 0, sizeof(uint) * counter_num);
	}
#endif

}

SC::~SC()
{
	for(int i = 0; i < bucket_num; i++)
	{
		delete []ori_pkt[i];
		delete []FP[i];
		delete []counter[i];
#ifdef LRU
		delete []myclock[i];
#endif
	}
	delete []ori_pkt;
	delete []FP;
	delete []counter;
	delete []cur_pos;
#ifdef LRU
	delete []myclock;
#endif
}


void SC::init_spa(SPA * _spa)
{
	spa = _spa;
}

void SC::Insert(pkt pkt_t, int f)
{
#ifdef LRU
	time_now ++;
#endif

#ifdef SIP_DIP_PKT
	uint fp = pkt_t.sip ^ pkt_t.dip;
#endif
#ifdef FOUR_TUPLE_PKT
	uint fp = pkt_t.sip ^ pkt_t.dip ^ pkt_t.sport ^ pkt_t.dport;
#endif	
#ifdef FIVE_TUPLE_PKT
	uint fp = pkt_t.sip ^ pkt_t.dip ^ pkt_t.sport ^ pkt_t.dport ^ pkt_t.type;
#endif



	int bucket_id = fp % bucket_num;

	const __m128i item = _mm_set1_epi32((int)fp);

	__m128i *keys_p = (__m128i *)FP[bucket_id];

	__m128i a_comp = _mm_cmpeq_epi32(item, keys_p[0]);
	__m128i b_comp = _mm_cmpeq_epi32(item, keys_p[1]);
	__m128i c_comp = _mm_cmpeq_epi32(item, keys_p[2]);
	__m128i d_comp = _mm_cmpeq_epi32(item, keys_p[3]);

	a_comp = _mm_packs_epi32(a_comp, b_comp);
	c_comp = _mm_packs_epi32(c_comp, d_comp);
	a_comp = _mm_packs_epi32(a_comp, c_comp);

	int matched = _mm_movemask_epi8(a_comp);

	if(matched != 0)
	{
		//return 32 if input is zero;
		int matched_index = _tzcnt_u32((uint)matched);
		
		if(ori_pkt[bucket_id][matched_index] == pkt_t)
		{
			counter[bucket_id][matched_index] += f;
		}
		else
		{
			Insert_SC_SPA(ori_pkt[bucket_id][matched_index], counter[bucket_id][matched_index]);
			ori_pkt[bucket_id][matched_index] = pkt_t;
			counter[bucket_id][matched_index] = f;
		}
#ifdef LRU
		myclock[bucket_id][matched_index] = time_now;
#endif

		return;
	}

	

	int cur_pos_now = cur_pos[bucket_id];
	if(cur_pos_now != counter_num)
	{
		ori_pkt[bucket_id][cur_pos_now] = pkt_t;
		FP[bucket_id][cur_pos_now] = fp;
		counter[bucket_id][cur_pos_now] = f;

#ifdef LRU
		myclock[bucket_id][cur_pos_now] = time_now;
#endif

		cur_pos[bucket_id]++;
		return;
	}



#ifdef LRU
	int kick_index = 0;
	int min_time = (1 << 30);
	for(int i = 0; i < counter_num; i++)
	{
		if(myclock[bucket_id][i] < min_time)
		{
			min_time = myclock[bucket_id][i];
			kick_index = i;
		}
	}
	Insert_SC_SPA(ori_pkt[bucket_id][kick_index], counter[bucket_id][kick_index]);
	ori_pkt[bucket_id][kick_index] = pkt_t;
	FP[bucket_id][kick_index] = fp;
	counter[bucket_id][kick_index] = f;

	myclock[bucket_id][kick_index] = time_now;
#endif


#ifdef GRR

	Insert_SC_SPA(ori_pkt[bucket_id][cur_kick], counter[bucket_id][cur_kick]);
	ori_pkt[bucket_id][cur_kick] = pkt_t;
	FP[bucket_id][cur_kick] = fp;
	counter[bucket_id][cur_kick] = f;


	cur_kick = (cur_kick + 1) % counter_num;

#endif


	
}
void SC::Insert_SC_SPA(pkt pkt_t, int kick_f)
{
	spa->Insert(pkt_t, kick_f);
}
void SC::refresh()
{
	for(int i = 0; i < bucket_num; i++)
	{
		for(int j = 0; j < counter_num; j++)
		{
			Insert_SC_SPA(ori_pkt[i][j], counter[i][j]);

#ifdef SIP_DIP_PKT
			ori_pkt[i][j].sip = ori_pkt[i][j].dip = 0;
#endif
#ifdef FOUR_TUPLE_PKT
			ori_pkt[i][j].sip = ori_pkt[i][j].dip = ori_pkt[i][j].sport = ori_pkt[i][j].dport = 0;
#endif	
#ifdef FIVE_TUPLE_PKT
			ori_pkt[i][j].sip = ori_pkt[i][j].dip = ori_pkt[i][j].sport = ori_pkt[i][j].dport = ori_pkt[i][j].type = 0;
#endif



			FP[i][j] = counter[i][j] = 0;
		}
		cur_pos[i] = 0;
	}
	return;
}

#endif//_SC_H