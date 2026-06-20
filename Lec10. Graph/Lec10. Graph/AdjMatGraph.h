#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include "Node.h"
#include "Heap.h"
#include "VertexSets.h"
#define MAX_VTXS 20
#define INF 9999

class AdjMatGraph {
protected:
    int size;
    char vertices[MAX_VTXS];
    int adj[MAX_VTXS][MAX_VTXS];
public:
    AdjMatGraph() : size(0), vertices{}, adj{} { reset(); }
    char getVertex(int i) { return vertices[i]; }
    int getEdge(int i, int j) { return adj[i][j]; }
    void setEdge(int i, int j, int val) { adj[i][j] = val; }
    bool isEmpty() { return size == 0; }
    bool isFull() { return size >= MAX_VTXS; }

    void reset() {
        size = 0;
        for (int i = 0; i < MAX_VTXS; i++)
            for (int j = 0; j < MAX_VTXS; j++)
                setEdge(i, j, 0);
    }

    void insertVertex(char name) {
        if (!isFull()) vertices[size++] = name;
        else printf("Error : 그래프 정점 개수 초과\n");
    }

    void insertEdge(int u, int v) {
        setEdge(u, v, 1);
        setEdge(v, u, 1);
    }

    void display(FILE* fp = stdout) {
        fprintf(fp, "%d\n", size);
        for (int i = 0; i < size; i++) {
            fprintf(fp, "%c\n", getVertex(i));
            for (int j = 0; j < size; j++)
                fprintf(fp, "%3d", getEdge(i, j));
            fprintf(fp, "\n");
        }
    }
};


class AdjListGraph {
protected:
    int size;
    char vertices[MAX_VTXS];
    Node* adj[MAX_VTXS];

public:
    AdjListGraph() : size(0), vertices{} {
        for (int i = 0; i < MAX_VTXS; i++) {
            adj[i] = NULL;
        }
    }

    ~AdjListGraph() { reset(); }

    bool isFull() { return size >= MAX_VTXS; }
    bool isEmpty() { return size == 0; }
    char getVertex(int i) { return vertices[i]; }

    void reset(void) {
        for (int i = 0; i < size; i++) {
            Node* curr = adj[i];
            while (curr != NULL) {
                Node* next = curr->getLink();
                delete curr;
                curr = next;
            }
            adj[i] = NULL;
        }
        size = 0;
    }

    void insertVertex(char val) {
        if (!isFull()) {
            vertices[size] = val;
            adj[size++] = NULL;
        }
        else {
            printf("Error : 그래프 정점 개수 초과\n");
        }
    }

    void insertEdge(int u, int v) {
        adj[u] = new Node(v, adj[u]);
        adj[v] = new Node(u, adj[v]);
    }

    void display() {
        printf("%d\n", size);
        for (int i = 0; i < size; i++) {
            printf("%c", getVertex(i));
            for (Node* v = adj[i]; v != NULL; v = v->getLink()) {
                printf(" -> %c", getVertex(v->getId()));
            }
            printf("\n");
        }
    }

    Node* adjacent(int v) { return adj[v]; }
};


class Wgraph : public AdjMatGraph {
public:
    Wgraph() { reset(); }

    void reset() {
        size = 0;
        for (int i = 0; i < MAX_VTXS; i++)
            for (int j = 0; j < MAX_VTXS; j++)
                setEdge(i, j, (i == j) ? 0 : INF);
    }

    void insertEdge(int u, int v, int weight) {
        if (weight > INF) weight = INF;
        setEdge(u, v, weight);
        setEdge(v, u, weight);
    }

    void display(FILE* fp = stdout) {
        fprintf(fp, "%d\n", size);
        for (int i = 0; i < size; i++) {
            fprintf(fp, "%c ", getVertex(i));
            for (int j = 0; j < size; j++) {
                int w = getEdge(i, j);
                if (w == INF) fprintf(fp, "  INF");
                else          fprintf(fp, "%5d", w);   // 폭을 5로 늘림
            }
            fprintf(fp, "\n");
        }
    }


    bool hasEdge(int i, int j) {
        return (i != j && getEdge(i, j) < INF);
    }

    void load(char* filename) {
        FILE* fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("Error: 파일 열기 실패 (%s)\n", filename);
            return;
        }
        int n, val;
        (void)fscanf(fp, "%d", &n);
        for (int i = 0; i < n; i++) {
            char str[80];
            (void)fscanf(fp, "%s", str);
            insertVertex(str[0]);
            for (int j = 0; j < n; j++) {
                (void)fscanf(fp, "%d", &val);
                if (i != j) insertEdge(i, j, val);
            }
        }
        fclose(fp);
    }
};

class WGraphMST : public Wgraph {
public:
    void Kruskal() {
        MinHeap heap;
		for (int i = 0; i < size; i++)
        for(int j = i + 1; j < size; j++)
                if (hasEdge(i, j))
                    heap.insert(getEdge(i, j), i, j);
        VertexSets set(size);
		int edgeAccepted = 0;
        while (edgeAccepted < size - 1) {
			HeapNode& e = heap.remove();
			int uset = set.findSet(e.getV1());
			int vset = set.findSet(e.getV2());
			if (uset != vset) {
				printf("간선 추가 :%c - %c (비용 : %d)\n", getVertex(e.getV1()), getVertex(e.getV2()), e.getKey());
                set.unionSets(uset, vset);
                edgeAccepted++;
            }
        }              
                
    }
};