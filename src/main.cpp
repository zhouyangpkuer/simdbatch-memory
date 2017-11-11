#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <string.h>
#include <ctime>
#include <time.h>
#include <iterator>
#include <math.h>
#include <vector>
#include <algorithm>
#include "params.h"
#include "SPA.h"



#ifdef SIP_PKT
#include "SC_short_key.h"
#endif

#ifdef DIP_PKT
#include "SC_short_key.h"
#endif

#ifdef SIP_DIP_PKT
#include "SC_long_key.h"
#include "TwoLevel.h"
#endif

#ifdef FOUR_TUPLE_PKT
#include "SC_long_key.h"
#endif

#ifdef FIVE_TUPLE_PKT
#include "SC_long_key.h"
#endif




#include "CMSketch.h"
#include "FlowRadar.h"
#include "SpaceSaving.h"
#include "FM.h"
#include "RevSketch.h"
#include "MRAC.h"
#include "UnivMon.h"
#include "LC.h"


#define TESTCYCLES 1


typedef pair<int, pkt> PAIR;  
int cmp(const PAIR &x, const PAIR &y)  
{ 
    return x.first > y.first;  
}

using namespace std;



pkt insert[MAX_INSERT_PACKAGE + MAX_INSERT_PACKAGE / 500];
pkt query[MAX_INSERT_PACKAGE + MAX_INSERT_PACKAGE / 500];

unordered_map<pkt, int> mp;

int main(int argc, char** argv)
{
    char filename_FlowTraffic[100];
    strcpy(filename_FlowTraffic, "./traffic/traffic_merge.csv");
    FILE *file_FlowTraffic = fopen(filename_FlowTraffic, "r");
    
    printf("PKT_LEN = %d\n", PKT_LEN);

#ifdef LRU
    printf("LRU\n");
#endif

#ifdef GRR
    printf("GRR\n");
#endif



#ifdef SIP_PKT
    printf("SIP_PKT\n");
#endif

#ifdef DIP_PKT
    printf("DIP_PKT\n");
#endif
    
#ifdef SIP_DIP_PKT
    printf("SIP_DIP_PKT\n");
#endif
    
#ifdef FOUR_TUPLE_PKT
    printf("FOUR_TUPLE_PKT\n");
#endif
    
#ifdef FIVE_TUPLE_PKT
    printf("FIVE_TUPLE_PKT\n");
#endif
    


    int package_num = 0;
    
    uint sip, dip;
    usi sport, dport;
    usi type;

    char buf[600];
    fgets(buf, 500, file_FlowTraffic);

    while(fscanf(file_FlowTraffic, "%u,%u,%hu,%hu,%hu", &sip, &dip, &sport, &dport, &type) != EOF)
    {

        // printf("type = %d\n", type);
#ifdef SIP_PKT
        insert[package_num].sip = sip;
#endif

#ifdef DIP_PKT
        insert[package_num].dip = dip;
#endif

#ifdef SIP_DIP_PKT
        insert[package_num].sip = sip;
        insert[package_num].dip = dip;
        if(sip == 0 || dip == 0 || sip == 0xFFFFFFFF || dip == 0xFFFFFFFF)
        {
            printf("happens!!!\n");
        }
#endif


#ifdef FOUR_TUPLE_PKT
        insert[package_num].sip = sip;
        insert[package_num].dip = dip;
        insert[package_num].sport = sport;
        insert[package_num].dport = dport;
#endif

#ifdef FIVE_TUPLE_PKT
        insert[package_num].sip = sip;
        insert[package_num].dip = dip;
        insert[package_num].sport = sport;
        insert[package_num].dport = dport;
        insert[package_num].type = type;
#endif

        mp[insert[package_num]]++;
        package_num++;

        if(package_num == MAX_INSERT_PACKAGE)
            break;
    }
    fclose(file_FlowTraffic);

    printf("dataset: %s\n", filename_FlowTraffic);
    printf("total stream size = %d\n", package_num);
    printf("distinct item number = %d\n", mp.size());

    int max_freq = 0;
    unordered_map<pkt, int>::iterator it = mp.begin();

    for(int i = 0; i < mp.size(); i++, it++)
    {
        query[i] = it->first;
        int temp2 = it->second;
        max_freq = max_freq > temp2 ? max_freq : temp2;
    }
    printf("max_freq = %d\n", max_freq);

    vector<pair<int, pkt>> vtMap;
    for (it = mp.begin(); it != mp.end(); it++)
        vtMap.push_back(make_pair(it->second, it->first));

    sort(vtMap.begin(), vtMap.end(), 
        [](const pair<int, pkt> &x, const pair<int, pkt> &y) -> int {
        return x.first > y.first;
    });







    double memory = 1.0;
    int d = 4;
    int bucket_num = 2000;
    if(argc == 2)
    {
        bucket_num = atoi(argv[1]);
    }

    int counter_num = 16;
    double M_SIMD = bucket_num * counter_num * 8.0 / (1024 * 1024);
    printf("M_SIMD = %lf\n", M_SIMD);


    SC * sc = NULL;


    timespec time1, time2;
    long long resns;
    double throughput;

#ifdef CMS



    CMSketch * cmsketch = NULL;
    CMSketch * cmsketch_sc = NULL;


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        cmsketch = new CMSketch(memory, d);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            cmsketch->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of CM (insert): %.6lf\n", throughput);
    printf("update_num of CM (insert): %d\n", (int)MAX_INSERT_PACKAGE);

    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        cmsketch_sc = new CMSketch(memory, d);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(cmsketch_sc);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of CM_SC (insert): %.6lf\n", throughput);
    printf("update_num of CM_SC (insert): %d\n", sc->update_num);

    printf("update percentage = %.6lf\n", sc->update_num * 1.0 / MAX_INSERT_PACKAGE);
    printf("ideal percentage = %.6lf\n", mp.size() * 1.0 / MAX_INSERT_PACKAGE);

#endif //CM


#ifdef FR

    FlowRadar * fr = NULL;
    FlowRadar * fr_sc = NULL;


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        fr = new FlowRadar(0.2 * memory, 0.8 * memory, d, d);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            fr->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of FR (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        fr_sc = new FlowRadar(0.2 * memory, 0.8 * memory, d, d);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(fr_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of FR_SC (insert): %.6lf\n", throughput);
    
#endif //FR

#ifdef SS

    SpaceSaving *ss = NULL;
    SpaceSaving *ss_sc = NULL;
    int K = 128;


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        ss = new SpaceSaving(K);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            ss->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of SS (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        // fr_sc = new FlowRadar(0.2 * memory, 0.8 * memory, d, d);
        ss_sc = new SpaceSaving(K);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(ss_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of SS_SC (insert): %.6lf\n", throughput);

    int cor_cnt = 0;
    pkt * result = new pkt[K];
    ss->get_top_k(K, result);
    for(int i = 0; i < K; i++)
    {
        for(int j = 0; j < K; j++)
        {
            if(vtMap[j].second == result[i])
            {
                cor_cnt++;
                break;
            }
        }
    }
    printf("%d/%d\n", cor_cnt, K);

    ss_sc->get_top_k(K, result);
    for(int i = 0; i < K; i++)
    {
        for(int j = 0; j < K; j++)
        {
            if(vtMap[j].second == result[i])
            {
                cor_cnt++;
                break;
            }
        }
    }
    printf("%d/%d\n", cor_cnt, K);

    
#endif //SS


#ifdef FM

    FMSketch * fm = NULL;
    FMSketch * fm_sc = NULL;



    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        fm = new FMSketch(d, 32);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            fm->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of FM (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        fm_sc = new FMSketch(d, 32);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(fm_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of FM_SC (insert): %.6lf\n", throughput);

    
#endif //FM


#ifdef REV

    RevSketch * rev = NULL;
    RevSketch * rev_sc = NULL;



    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        rev = new RevSketch(memory, d);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            rev->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of REV (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        rev_sc = new RevSketch(memory, d);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(rev_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of REV_SC (insert): %.6lf\n", throughput);

    
#endif //REV




#ifdef TWO

    TwoLevel * two = NULL;
    TwoLevel * two_sc = NULL;



    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        two = new TwoLevel(memory, d);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            two->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of TWO (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        two_sc = new TwoLevel(memory, d);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(two_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of TWO_SC (insert): %.6lf\n", throughput);

    
#endif //TWO


#ifdef MRAC_DIS

    MRAC * mrac = NULL;
    MRAC * mrac_sc = NULL;



    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        mrac = new MRAC(memory, 10);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            mrac->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of MRAC (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        mrac_sc = new MRAC(memory, 10);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(mrac_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of MRAC_SC (insert): %.6lf\n", throughput);

    
#endif //MRAC_DIS



#ifdef UNIV

    UnivMon * umon = NULL;
    UnivMon * umon_sc = NULL;



    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        umon = new UnivMon(memory, 4);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            umon->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of UMON (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        umon_sc = new UnivMon(memory, 4);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(umon_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of UMON_SC (insert): %.6lf\n", throughput);

    
#endif //UNIV



#ifdef LCNT

    LC * lc = NULL;
    LC * lc_sc = NULL;



    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        lc = new LC(memory, 4);
        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            lc->Insert(insert[i], 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of LC (insert): %.6lf\n", throughput);
    


    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        lc_sc = new LC(memory, 4);
        sc = new SC(bucket_num, counter_num);
        sc->init_spa(lc_sc);

        for(int i = 0; i < MAX_INSERT_PACKAGE; i++)
        {
            sc->Insert(insert[i], 1);
        }
        sc->refresh();
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput = (double)1000.0 * TESTCYCLES * package_num / resns;
    printf("throughput of LC_SC (insert): %.6lf\n", throughput);

    
#endif //LCNT



    /*
    int flow_num = mp.size();
    int temp = 0;
    int sum = 0;

    clock_gettime(CLOCK_MONOTONIC, &time1);

    for(int x = 0; x < TESTCYCLES; x++)
    {
        for(int i = 0; i < flow_num; i++)
        {
            temp += cmsketch->Query(query[i]);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_cu_q = (double)1000.0 * TESTCYCLES * flow_num / resns;
    printf("throughput of CM (query): %.6lf\n", throughput_cu_q);
    
    sum += temp;



    clock_gettime(CLOCK_MONOTONIC, &time1);
    
    for(int x = 0; x < TESTCYCLES; x++)
    {
        for(int i = 0; i < flow_num; i++)
        {
            temp += cmsketch_sc->Query(query[i]);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_cu_sc_q = (double)1000.0 * TESTCYCLES * flow_num / resns;
    printf("throughput of CM_SC (query): %.6lf\n", throughput_cu_sc_q);
    
    sum += temp;

    if(sum == (1 << 30))
        return 0;





    


    vector<PAIR> pair_vec;


    double AE1 = 0.0, AE2 = 0.0;
    int i = 0;
    pkt temp_pkt;

    FILE * file_res = fopen("result.txt", "w");
    for(unordered_map<pkt, int>::iterator it = mp.begin(); it != mp.end();i++, it++)
    {
        temp_pkt = it->first;
        int val = it->second;
        
        pair_vec.push_back(make_pair(val, temp_pkt));  


        int correct_cnt1 = cmsketch->Query(temp_pkt);
        AE1 += correct_cnt1 - val;

        int correct_cnt2 = cmsketch_sc->Query(temp_pkt);
        AE2 += correct_cnt2 - val;

        fprintf(file_res, "%d, %d, %d\n", val, correct_cnt1, correct_cnt2);
    }
    fclose(file_res);

    printf("AE1 = %lf\n", AE1 / flow_num);
    printf("AE2 = %lf\n", AE2 / flow_num);

    */

    /*
    sort(pair_vec.begin(), pair_vec.end(), cmp);  

    for(int i = 0; i < 10; i++)
    {
        int val = pair_vec[i].first;
        temp_pkt = pair_vec[i].second;


#ifdef SIP_PKT
        printf("%u-->%d\n", temp_pkt.sip, val);
#endif
#ifdef DIP_PKT
        printf("%u-->%d\n", temp_pkt.dip, val);
#endif
#ifdef SIP_DIP_PKT
        printf("%u,%u-->%d\n", temp_pkt.sip, temp_pkt.dip, val);
#endif
#ifdef FOUR_TUPLE_PKT
        printf("%u,%u,%hu,%hu-->%d\n", temp_pkt.sip, temp_pkt.dip, temp_pkt.sport, temp_pkt.dport, val);
#endif
#ifdef FIVE_TUPLE_PKT
        printf("%u,%u,%hu,%hu,%hu-->%d\n", temp_pkt.sip, temp_pkt.dip, temp_pkt.sport, temp_pkt.dport, temp_pkt.type, val);
#endif

    }
    */


    return 0;

}
