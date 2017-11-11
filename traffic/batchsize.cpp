#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unordered_map>
#include <string>
using namespace std;

typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

#define MAX_PKT_NUM (14 * 2 * 1024 * 1024)

string keys[MAX_PKT_NUM + 1024];
uint64_t keys_sd[MAX_PKT_NUM + 1024];


int main()
{
	FILE * file_sender = fopen("traffic_sender.dat", "r");
	uint32_t sip, dip, sport, dport, type;
	uint64_t key;
	char buf[200];
	uint32_t pkt_num = 0;



	fscanf(file_sender, "%s\n", buf);

	// while(fscanf(file_sender, "%u\t%u\t%u\t%u\t%u\n", &sip, &dip, &sport, &dport, &type) != EOF)
	// while(fscanf(file_sender, "%s\n", buf) != EOF)
    while(fgets(buf, 200, file_sender) != NULL)
	{
		// printf("%s", buf);

		sscanf(buf, "%u\t%u\t%u\t%u\t%u", &sip, &dip, &sport, &dport, &type);

		// printf("%u\t%u\n", sport, dport);

		uint32_t * p = (uint32_t *)&key;
		memcpy(p, &sip, 4);
		memcpy(p + 1, &dip, 4);
		keys_sd[pkt_num] = key;
		keys[pkt_num] = string(buf);

		pkt_num ++;
		if(pkt_num == MAX_PKT_NUM)
			break;
	}
	fclose(file_sender);
	printf("readfile complete!\n");


	FILE * file_res = fopen("flownum_batchsize.txt", "w");

	// int batchsize[11] = {32, 64, 128, 256, 512, 768, 1024, 1280, 1536, 1792, 2048};

	for(int batchsize = 32; batchsize <= 2048; batchsize <<= 1)
	{
		unordered_map<string, uint32_t> mp;
		unordered_map<uint64_t, uint32_t> mp_sd;

		double avg_flow_num = 0.0, avg_flow_num_sd = 0.0;
		uint32_t batch_num = 0;
		for(int i = 0; i < MAX_PKT_NUM; i++)
		{
			mp[keys[i]] ++;
			mp_sd[keys_sd[i]] ++;
			if((i + 1) % batchsize == 0)
			{
				batch_num ++;
				avg_flow_num += mp.size();
				avg_flow_num_sd += mp_sd.size();
				mp.clear();
				mp_sd.clear();
			}
		}
		avg_flow_num /= batch_num;
		avg_flow_num_sd /= batch_num;

		printf("%d\t%lf\t%lf\t%lf\t%lf\n", batchsize, avg_flow_num, avg_flow_num / batchsize, avg_flow_num_sd, avg_flow_num_sd / batchsize);
		fprintf(file_res, "%d\t%lf\t%lf\t%lf\t%lf\n", batchsize, avg_flow_num, avg_flow_num / batchsize, avg_flow_num_sd, avg_flow_num_sd / batchsize);
	}
	fclose(file_res);
	return 0;
}