#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const int MAX_VTXS = 100;
const int INF = 9999;
const int MAX_HEAP_NODE = MAX_VTXS * MAX_VTXS;

// ===============================
// 인접 행렬 그래프 클래스
// ===============================
class AdjMatGraph {
protected:
    int  size;                       // 정점 개수
    char vertices[MAX_VTXS];         // 정점 이름
    int  adj[MAX_VTXS][MAX_VTXS];    // 인접 행렬

public:
    AdjMatGraph() { reset(); }

    void reset() {
        size = 0;
        for (int i = 0; i < MAX_VTXS; i++)
            for (int j = 0; j < MAX_VTXS; j++)
                adj[i][j] = (i == j) ? 0 : INF;
    }

    bool isFull() const { return size >= MAX_VTXS; }

    void insertVertex(char name) {
        if (isFull()) { cout << "정점 개수 초과" << endl; return; }
        vertices[size++] = name;
    }

    char getVertex(int i) const { return vertices[i]; }
    int  getEdge(int i, int j) const { return adj[i][j]; }
    void setEdge(int i, int j, int value) { adj[i][j] = value; }
    int  getSize() const { return size; }

    void display(const char* msg = "Graph") const {
        cout << msg << endl << endl;

        cout << "     ";
        for (int i = 0; i < size; i++) cout << setw(5) << vertices[i];
        cout << endl;

        for (int i = 0; i < size; i++) {
            cout << setw(5) << vertices[i];
            for (int j = 0; j < size; j++) {
                if (adj[i][j] >= INF) cout << setw(5) << "INF";
                else                  cout << setw(5) << adj[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }
};

// ===============================
// 가중치 그래프 클래스 (무방향)
// ===============================
class WGraph : public AdjMatGraph {
public:
    // 무방향이므로 양쪽 모두 설정
    void insertEdge(int u, int v, int weight) {
        if (weight > INF) weight = INF;
        setEdge(u, v, weight);
        setEdge(v, u, weight);
    }

    bool hasEdge(int i, int j) const {
        return (i != j) && (getEdge(i, j) < INF);
    }

    void load(const char* filename) {
        ifstream fin(filename);
        if (!fin) {
            cout << "파일을 열 수 없습니다: " << filename << endl;
            return;
        }

        reset();

        int n;
        fin >> n;
        if (n > MAX_VTXS) {
            cout << "정점 개수가 너무 많습니다." << endl;
            return;
        }

        for (int i = 0; i < n; i++) {
            char name;
            fin >> name;
            insertVertex(name);

            for (int j = 0; j < n; j++) {
                int value;
                fin >> value;
                // 대각선은 0 유지, 그 외에는 가중치 설정
                if (i != j) setEdge(i, j, (value >= INF) ? INF : value);
            }
        }
        fin.close();
    }
};

// ===============================
// Union-Find (VertexSets)
// ===============================
class VertexSets {
private:
    int parent[MAX_VTXS];
    int nSets;

public:
    VertexSets(int n) : nSets(n) {
        for (int i = 0; i < nSets; i++) parent[i] = -1; // 자기 자신이 루트
    }

    bool isRoot(int i) const { return parent[i] < 0; }

    int findSet(int v) const {
        while (!isRoot(v)) v = parent[v];
        return v;
    }

    // 주의: 두 인자는 반드시 '루트'여야 함 (Kruskal에서 findSet 결과를 넘김)
    void unionSets(int s1, int s2) {
        if (s1 == s2) return;
        parent[s1] = s2;
        nSets--;
    }

    int getCount() const { return nSets; }
};

// ===============================
// HeapNode (간선)
// ===============================
class HeapNode {
private:
    int key;   // 간선의 가중치
    int v1;    // 정점 1
    int v2;    // 정점 2

public:
    HeapNode(int k = 0, int u = 0, int v = 0) : key(k), v1(u), v2(v) {}

    void setKey(int k, int u, int v) { key = k; v1 = u; v2 = v; }
    int  getKey() const { return key; }
    int  getV1()  const { return v1; }
    int  getV2()  const { return v2; }
};

// ===============================
// 최소 힙
// ===============================
class MinHeap {
private:
    HeapNode node[MAX_HEAP_NODE];
    int      size;

public:
    MinHeap() : size(0) {}

    bool isEmpty() const { return size == 0; }
    bool isFull()  const { return size >= MAX_HEAP_NODE - 1; }

    HeapNode& getParent(int i) { return node[i / 2]; }

    void insert(int key, int u, int v) {
        if (isFull()) { cout << "Heap is full." << endl; return; }

        int i = ++size;
        while (i != 1 && key < getParent(i).getKey()) {
            node[i] = getParent(i);
            i /= 2;
        }
        node[i].setKey(key, u, v);
    }

    HeapNode remove() {
        if (isEmpty()) { cout << "Heap is empty." << endl; return HeapNode(); }

        HeapNode item = node[1];
        HeapNode last = node[size--];

        int parent = 1, child = 2;
        while (child <= size) {
            if (child < size &&
                node[child].getKey() > node[child + 1].getKey())
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

// ===============================
// MST 기능 추가 (Kruskal + Prim)
// ===============================
class WGraphMST : public WGraph {
public:
    // -------- Kruskal --------
    void Kruskal() {
        MinHeap heap;

        // 1. 모든 간선을 최소힙에 삽입 (i<j 만 — 무방향)
        for (int i = 0; i < size - 1; i++)
            for (int j = i + 1; j < size; j++)
                if (hasEdge(i, j))
                    heap.insert(getEdge(i, j), i, j);

        VertexSets set(size);
        int edgeAccepted = 0;
        int totalCost = 0;

        while (edgeAccepted < size - 1) {
            if (heap.isEmpty()) {
                cout << "그래프가 연결되어 있지 않아 MST를 만들 수 없습니다." << endl;
                return;
            }

            HeapNode e = heap.remove();
            int uset = set.findSet(e.getV1());
            int vset = set.findSet(e.getV2());

            if (uset != vset) {
                cout << "간선 추가 : "
                    << getVertex(e.getV1()) << " - "
                    << getVertex(e.getV2())
                    << " (비용:" << e.getKey() << ")" << endl;

                set.unionSets(uset, vset);
                edgeAccepted++;
                totalCost += e.getKey();
            }
            // 같은 집합이면 사이클 발생 → 버림
        }

        cout << "MST 총 비용 : " << totalCost << endl;
    }

    // -------- Prim --------
    void Prim() {
        MinHeap heap;
        bool selected[MAX_VTXS] = { false };
        int  totalCost = 0;
        int  edgesAccepted = 0;

        // 1. 시작 정점 0
        selected[0] = true;
        int last = 0;

        while (edgesAccepted < size - 1) {
            // 2. 새로 포함된 정점(last)의 외부 간선만 힙에 push
            for (int j = 0; j < size; j++) {
                if (!selected[j] && hasEdge(last, j))
                    heap.insert(getEdge(last, j), last, j);
            }

            // 3. 아직 selected 안 된 정점으로 가는 최소 간선 선택
            int u = -1, v = -1, w = 0;
            while (!heap.isEmpty()) {
                HeapNode e = heap.remove();
                if (!selected[e.getV2()]) {
                    u = e.getV1();
                    v = e.getV2();
                    w = e.getKey();
                    break;
                }
                // 양쪽 다 selected → 사이클, 버림
            }

            if (v == -1) {
                cout << "그래프가 연결되어 있지 않아 MST를 만들 수 없습니다." << endl;
                return;
            }

            cout << "간선 추가 : "
                << getVertex(u) << " - " << getVertex(v)
                << " (비용:" << w << ")" << endl;

            selected[v] = true;
            totalCost += w;
            edgesAccepted++;
            last = v;   // 다음 라운드 push 기준
        }

        cout << "MST 총 비용 : " << totalCost << endl;
    }
};
