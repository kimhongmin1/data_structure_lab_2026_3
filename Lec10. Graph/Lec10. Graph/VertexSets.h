#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#define MAX_VTXS 20

class VertexSets {
    int parent[MAX_VTXS];
    int n;
public:
    VertexSets(int size) : n(size) {
        for (int i = 0; i < n; i++) parent[i] = -1; // -1 : 자기 자신이 루트
    }

    // 경로 압축
    int findSet(int v) {
        if (parent[v] < 0) return v;
        parent[v] = findSet(parent[v]);
        return parent[v];
    }

    // 두 집합을 합침 (두 인자는 모두 '루트'여야 함 — Kruskal 코드에서 그렇게 호출함)
    void unionSets(int s1, int s2) {
        if (s1 == s2) return;
        // 크기 기반 합치기 (parent에 음수로 크기 저장)
        if (parent[s1] <= parent[s2]) {
            parent[s1] += parent[s2];
            parent[s2] = s1;
        }
        else {
            parent[s2] += parent[s1];
            parent[s1] = s2;
        }
    }
};
