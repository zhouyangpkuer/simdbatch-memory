#ifndef _COUNTHEAP_H
#define _COUNTHEAP_H

#include <unordered_map>
#include <algorithm>
#include "BOBHash32.h"
#include "params.h"

using std::min;
using std::swap;

// template<int capacity, int cm_memory_in_bytes, int d>
class CountHeap {
    typedef pair <uint32_t, pkt> KV;
    KV *heap;
    int capacity, heap_element_num;
    int w, d;
    int **countsketch;
    BOBHash32 **hash;
    BOBHash32 ** hash_polar;
    unordered_map<pkt, uint32_t> ht;

    // heap
    void heap_adjust_down(int i) {
        while (i < heap_element_num / 2) {
            int l_child = 2 * i + 1;
            int r_child = 2 * i + 2;
            int larger_one = i;
            if (l_child < heap_element_num && heap[l_child] < heap[larger_one]) {
                larger_one = l_child;
            }
            if (r_child < heap_element_num && heap[r_child] < heap[larger_one]) {
                larger_one = r_child;
            }
            if (larger_one != i) {
                swap(heap[i], heap[larger_one]);
                swap(ht[heap[i].second], ht[heap[larger_one].second]);
                heap_adjust_down(larger_one);
            } else {
                break;
            }
        }
    }

    void heap_adjust_up(int i) {
        while (i > 1) {
            int parent = (i - 1) / 2;
            if (heap[parent] <= heap[i]) {
                break;
            }
            swap(heap[i], heap[parent]);
            swap(ht[heap[i].second], ht[heap[parent].second]);
            i = parent;
        }
    }

public:
    //Count sketch memory; hash function number; heap size;
    CountHeap(double memory, int _d, int _capacity) : heap_element_num(0), d(_d), capacity(_capacity) {
        heap = new KV[capacity];
        memset(heap, 0, sizeof(heap));

        w = memory * 1024 * 1024 * 8.0 / 32 / d;

        countsketch = new int *[d];
        for(int i = 0; i < d; i++)
        {
            countsketch[i] = new int[w];
            memset(countsketch[i], 0, sizeof(int) * w);
        }

        hash = new BOBHash32 *[d];
        for (int i = 0; i < d; i++)
            hash[i] = new BOBHash32(i + 750);

        hash_polar = new BOBHash32 *[d];
        for (int i = 0; i < d; ++i)
            hash_polar[i] = new BOBHash32(i + 100);
    }

    void Insert(pkt key, int f) {
        int ans[MAX_HASH_NUM];

        for (int i = 0; i < d; ++i) {
            int idx = hash[i]->run((const char *)&key, PKT_LEN) % w;
            int polar = hash_polar[i]->run((const char *)&key, PKT_LEN) % 2;

            countsketch[i][idx] += polar ? f : (-1 * f);

            int val = countsketch[i][idx];

            ans[i] = polar ? val : -val;
        }

        sort(ans, ans + d);

        uint32_t tmin;
        if (d % 2 == 0) {
            tmin = (ans[d / 2] + ans[d / 2] - 1) / 2;
        } else {
            tmin = ans[d / 2];
        }

        if (ht.find(key) != ht.end()) {
            heap[ht[key]].first += f;
            heap_adjust_down(ht[key]);
        } else if (heap_element_num < capacity) {
            heap[heap_element_num].second = key;
            heap[heap_element_num].first = tmin;
            ht[key] = heap_element_num++;
            heap_adjust_up(heap_element_num - 1);
        } else if (tmin > heap[0].first) {
            KV & kv = heap[0];
            ht.erase(kv.second);
            kv.second = key;
            kv.first = tmin;
            ht[key] = 0;
            heap_adjust_down(0);
        }
    }

    /*
    void get_top_k(uint16_t k, uint32_t * result) {
        KV * a = new KV[capacity];
        memcpy(a, heap, sizeof(heap));
        sort(a, a + capacity);
        int i;
        for (i = 0; i < k && i < capacity; ++i) {
            result[i] = a[capacity - 1 - i].second;
        }
        for (; i < k; ++i) {
            result[i] = 0;
        }
    }

    void get_top_k_with_frequency(uint16_t k, vector<pair<uint32_t, uint32_t>> & result) {
        KV * a = new KV[capacity];
        memcpy(a, heap, sizeof(heap));
        sort(a, a + capacity);
        int i;
        for (i = 0; i < k && i < capacity; ++i) {
            result[i].first = a[capacity - 1 - i].second;
            result[i].second = a[capacity - 1 - i].first;
        }
        for (; i < k; ++i) {
            result[i].first = 0;
            result[i].second = 0;
        }
    }*/

    // void build(uint32_t * items, int n)
    // {
    //     for (int i = 0; i < n; ++i) {
    //         insert(items[i]);
    //     }
    // }

    ~CountHeap() {
        delete []heap;
        for (int i = 0; i < d; ++i) {
            delete hash[i];
            delete hash_polar[i];
            delete countsketch[i];
        }
        delete []hash;
        delete []hash_polar;
        delete []countsketch;
        return;
    }
};

#endif //_COUNTHEAP_H