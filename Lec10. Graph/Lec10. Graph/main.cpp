#include <cstdio>
#include "AdjMatGraph.h"

int main() {
    // ------------------------------------------------
    // 1. 인접 행렬 그래프 테스트
    // ------------------------------------------------
    AdjMatGraph matGraph;

    for (int i = 0; i < 4; i++)
        matGraph.insertVertex('A' + i);

    matGraph.insertEdge(0, 1);
    matGraph.insertEdge(0, 3);
    matGraph.insertEdge(1, 2);
    matGraph.insertEdge(1, 3);
    matGraph.insertEdge(2, 3);

    printf("=== 인접 행렬로 표현한 그래프 ===\n");
    matGraph.display();

    FILE* fp;
    if (fopen_s(&fp, "../text_out.txt", "w") == 0 && fp != NULL) {
        matGraph.display(fp);
        fclose(fp);
        printf("\n-> 인접 행렬 파일 저장 완료\n");
    }
    else {
        printf("\n-> 파일 저장 실패\n");
    }

    printf("\n");

    // ------------------------------------------------
    // 2. 인접 리스트 그래프 테스트
    // ------------------------------------------------
    AdjListGraph listGraph;

    for (int i = 0; i < 4; i++)
        listGraph.insertVertex('A' + i);

    listGraph.insertEdge(0, 1);
    listGraph.insertEdge(0, 3);
    listGraph.insertEdge(1, 2);
    listGraph.insertEdge(1, 3);
    listGraph.insertEdge(2, 3);

    printf("=== 인접 리스트로 표현한 그래프 ===\n");
    listGraph.display();

    printf("\n");

    // ------------------------------------------------
    // 3. Kruskal 최소 비용 신장 트리(MST) 테스트
    // ------------------------------------------------
    WGraphMST mst;

    for (int i = 0; i < 7; i++)
        mst.insertVertex('A' + i);   // A, B, C, D, E, F, G

    //  교재 예제 그래프 (가중치 그래프)
    mst.insertEdge(0, 1, 29);  // A-B
    mst.insertEdge(0, 5, 10);  // A-F
    mst.insertEdge(1, 2, 16);  // B-C
    mst.insertEdge(1, 6, 15);  // B-G
    mst.insertEdge(2, 3, 12);  // C-D
    mst.insertEdge(3, 4, 22);  // D-E
    mst.insertEdge(3, 6, 18);  // D-G
    mst.insertEdge(4, 5, 27);  // E-F
    mst.insertEdge(4, 6, 25);  // E-G

    printf("=== 가중치 그래프(인접 행렬) ===\n");
    mst.display();

    printf("\n=== Kruskal MST 수행 결과 ===\n");
    mst.Kruskal();

    return 0;
}
