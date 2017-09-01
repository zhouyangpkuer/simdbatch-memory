#ifndef _UNIVMON_H
#define _UNIVMON_H

#include "params.h"
#include "BOBHash32.h"
#include "SPA.h"
#include "CountHeap.h"
#include <string.h>
#include <stdlib.h>

using namespace std;


class UnivMon: public SPA
{
private:
	int sketch_num;
	CountHeap **CH;
	BOBHash32 ** bobhash;

public:
	UnivMon(double M, int _d);
	~UnivMon();
	virtual void Insert(pkt pkt_t, int f);

};
UnivMon::UnivMon(double M, int _d = 4)
{
	sketch_num = _d;
	CH = new CountHeap *[sketch_num];

	double total = (1 << sketch_num) - 1;
	for(int i = 0; i < sketch_num; i++)
	{
		CH[i] = new CountHeap(M * (1 << (sketch_num - 1 - i)) / total, 4, 500);
	}

	bobhash = new BOBHash32 *[sketch_num - 1];
	for(int i = 0; i < sketch_num - 1; i++)
		bobhash[i] = new BOBHash32(i + 750);

}
UnivMon::~UnivMon()
{
	for(int i = 0; i < sketch_num; i++)
		delete CH[i];
	delete[] CH;
	for(int i = 0; i < sketch_num - 1; i++)
		delete bobhash[i];
	delete[] bobhash;
}
void UnivMon::Insert(pkt pkt_t, int f)
{
	int polar;
	CH[0]->Insert(pkt_t, f);
	for(int i = 0; i < sketch_num - 1; i++)
	{
		polar = (bobhash[i]->run((const char *)&pkt_t, PKT_LEN)) % 2;
		if(polar == 1)
			CH[i + 1]->Insert(pkt_t, f);
		else
			break;
	}
	return;
}

#endif //_UNIVMON_H