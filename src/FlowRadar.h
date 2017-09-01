#ifndef _FLOWRADAR_H
#define _FLOWRADAR_H

#include "params.h" 
#include "BOBHash32.h"
#include "SPA.h"

using namespace std;

class FlowRadar: public SPA
{
private:
	int w_bf, w_ibf;
	int d_bf, d_ibf;
	
	struct node 
	{
		pkt flowx;
		int flowc;
		int packetc;
	};

	uint64_t * bf;
	node * ibf;

	BOBHash32 * bobhash[MAX_HASH_NUM];

public:
	FlowRadar(double Mbf, double Mibf, int dbf, int dibf)
	{
		w_bf = Mbf * 1024 * 1024 * 8.0 / 1;
		w_ibf = Mibf * 1024 * 1024 / sizeof(node);

		d_bf = dbf;
		d_ibf = dibf;

		bf = new uint64_t[w_bf >> 6];
		memset(bf, 0, sizeof(uint64_t) * (w_bf >> 6));
		ibf = new node[w_ibf];
		memset(ibf, 0, sizeof(node) * w_ibf);

		for (int i = 0; i < d_bf + d_ibf; i++)
		{
			bobhash[i] = new BOBHash32(i + 500);
		}
	}
	~FlowRadar()
	{
		delete []bf;
		delete []ibf;
		for(int i = 0; i < d_bf + d_ibf; i++)
			delete bobhash[i];
	}
	void Insert(pkt pkt_t, int f)
	{
	    int FLAG = 1;
	    int index[MAX_HASH_NUM];

	    for(int i = 0; i < d_bf; i++)
	    {
	    	index[i] = (bobhash[i]->run((const char *)&pkt_t, PKT_LEN)) % w_bf;
	    	if(((bf[index[i] >> 6] >> (index[i] & 0x3F)) & 1) == 0)
	    		FLAG = 0;

			bf[index[i] >> 6] |= ((uint64)1 << (index[i] & 0x3F));
	    }


	    for(int i = 0; i < d_ibf; i++)
	    {
	    	index[i] = (bobhash[i + d_bf]->run((const char *)&pkt_t, PKT_LEN)) % w_ibf;
	    	if(FLAG == 0)
	    	{

#ifdef SIP_PKT
    			ibf[index[i]].flowx.sip ^= pkt_t.sip;
#endif

#ifdef DIP_PKT
    			ibf[index[i]].flowx.dip ^= pkt_t.dip;
#endif
    
#ifdef SIP_DIP_PKT
    			ibf[index[i]].flowx.sip ^= pkt_t.sip;
    			ibf[index[i]].flowx.dip ^= pkt_t.dip;
#endif
    
#ifdef FOUR_TUPLE_PKT
    			ibf[index[i]].flowx.sip ^= pkt_t.sip;
    			ibf[index[i]].flowx.dip ^= pkt_t.dip;

    			ibf[index[i]].flowx.sport ^= pkt_t.sport;
    			ibf[index[i]].flowx.dport ^= pkt_t.dport;
#endif
    
#ifdef FIVE_TUPLE_PKT
    			ibf[index[i]].flowx.sip ^= pkt_t.sip;
    			ibf[index[i]].flowx.dip ^= pkt_t.dip;

    			ibf[index[i]].flowx.sport ^= pkt_t.sport;
    			ibf[index[i]].flowx.dport ^= pkt_t.dport;

    			ibf[index[i]].flowx.type ^= pkt_t.type;
#endif
    
	    		ibf[index[i]].flowc ++;
	    	}
	    	ibf[index[i]].packetc += f;
	    }
	}

};
#endif//_FLOWRADAR_H
