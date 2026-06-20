#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#define MAX_ELEMENT 200

class HeapNode {
    int key;   // 간선의 가중치
    int v1;    // 정점 1
    int v2;    // 정점 2
public:
    HeapNode(int k = 0, int u = 0, int v = 0) : key(k), v1(u), v2(v) {}
    void   setKey(int k) { key = k; }
    int    getKey() const { return key; }
    int    getV1()  const { return v1; }
    int    getV2()  const { return v2; }
    void   set(int k, int u, int v) { key = k; v1 = u; v2 = v; }
};

class MinHeap {
    HeapNode node[MAX_ELEMENT];
    int      size;
public:
    MinHeap() : size(0) {}
    bool isEmpty() { return size == 0; }
    bool isFull() { return size >= MAX_ELEMENT - 1; }

    void insert(int key, int u, int v) {
        if (isFull()) {
            printf("Error : 힙 용량 초과\n");
            return;
        }
        int i = ++size;
        // 상향 이동 (min-heap)
        while (i != 1 && key < node[i / 2].getKey()) {
            node[i] = node[i / 2];
            i /= 2;
        }
        node[i].set(key, u, v);
    }

    HeapNode& remove() {
        static HeapNode item;
        if (isEmpty()) {
            printf("Error : 힙이 비어있음\n");
            return item;
        }
        item = node[1];
        HeapNode last = node[size--];

        int parent = 1, child = 2;
        while (child <= size) {
            if (child < size && node[child].getKey() > node[child + 1].getKey())
                child++;
            if (last.getKey() <= node[child].getKey()) break;
            node[parent] = node[child];
            parent = child;
            child *= 2;
        }
        node[parent] = last;
        return item;
    }
};
