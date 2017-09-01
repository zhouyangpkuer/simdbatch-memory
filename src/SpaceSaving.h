#ifndef _SPACESAVING_H
#define _SPACESAVING_H

#include <cstdint>
#include <cstdlib>
#include "SPA.h"
#include "params.h"
#include <unordered_map>

using namespace std;

#define tail_node nodes[0].prev


struct Node
{
    pkt key;
    uint32_t val;
    Node * prev;
    Node * next;
    Node * parent;
};

// template<int capacity>
class SpaceSaving: public SPA
{
    int capacity;

    int now_element;
//    uint16_t tail_idx;
    Node *nodes;
    unordered_map<pkt, Node *> hash_table;

    void append_new_key(pkt pkt_t, int freq) {
        if (now_element < capacity) {
            uint16_t idx = ++now_element; // we use 0 to represent header
            nodes[idx].key = pkt_t;
            nodes[idx].val = 0;
            hash_table[pkt_t] = nodes + idx;

            // append to tail
            nodes[idx].prev = tail_node;
            tail_node->next = nodes + idx;
            nodes[idx].next = nodes;
            nodes[idx].parent = nodes + idx;
            tail_node = &nodes[idx];
            add_counter(tail_node, freq);
        } else {
            pkt old_key = tail_node->key;
            hash_table.erase(old_key);
            tail_node->key = pkt_t;
            hash_table[pkt_t] = tail_node;
            add_counter(tail_node, freq);
        }
    }

    void add_counter(Node * my, int freq) {
        //
        if (my->parent == my && my->next->val == my->val) {
//            std::swap(my->key, my->next->key);
//            std::swap(hash_table[my->key], hash_table[my->next->key]);
//            my = my->next;
            Node * p = my->next, * nt = my->next;
            while (p && p->val == my->val) {
                p->parent = nt;
                p = p->next;
            }
        }

        my->val += freq;
        int now_freq = my->val;
        Node * prev_node = my->prev;

        if (prev_node->val > now_freq) {
            return;
        }

        Node * next_node = my->next;

        // make next and prev connect
        prev_node->next = my->next;
        next_node->prev = my->prev;

        while (prev_node->val < now_freq) {
            prev_node = prev_node->parent->prev;
        }

        next_node = prev_node->next;

        my->next = prev_node->next;
        prev_node->next = my;

        my->prev = next_node->prev;
        next_node->prev = my;

        my->parent = (prev_node->val == my->val) ? prev_node->parent : my;
    }
public:
    SpaceSaving(int _capacity) : now_element(0) {
        capacity = _capacity;
        nodes = new Node[capacity + 1];
        memset(nodes, 0, sizeof(nodes));
        now_element = 0;
        nodes[0].val = -1;
        nodes[0].parent = nodes;
        tail_node = nodes;
        hash_table.reserve(100 * capacity);
    }
    ~SpaceSaving()
    {
        delete []nodes;
    }
    void Insert(pkt pkt_t, int f)
    {
//        Node *& my_node = hash_table[key];
        auto itr = hash_table.find(pkt_t);
        if (itr == hash_table.end()) {
            // key not found
            append_new_key(pkt_t, f);
        } else {
            // key found
            add_counter(itr->second, f);
        }
    }

    void get_top_k(uint16_t k, pkt * result) {
        Node * idx = nodes[0].next;

        for (int i = 0; i < k; ++i) {
            result[i] = idx->key;
            // printf("%d\t", idx->val);
            idx = idx->next;
        }
        return;
    }
};

#endif //_SPACESAVING_H
