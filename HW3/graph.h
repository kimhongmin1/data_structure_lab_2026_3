#ifndef GRAPH_H          // 헤더 중복 포함 방지 (인클루드 가드 시작)
#define GRAPH_H          // GRAPH_H가 정의 안 됐을 때만 아래 코드를 컴파일

#include <QString>       // 도시 이름 저장용 Qt 문자열 클래스
#include <QVector>       // 동적 배열(리스트) Qt 컨테이너
#include <QPointF>       // 화면상의 (x, y) 좌표를 실수로 저장하는 클래스

// ===== 간선(Edge) 구조체 =====
// 버스 노선 하나를 표현: 도착 도시 번호 + 소요시간 + 요금
struct Route {
    int to;        // 도착 도시의 인덱스 번호 (그래프에서 간선이 향하는 정점)
    int minutes;   // 소요 시간(분) — 간선의 가중치 1
    int fare;      // 요금(원)      — 간선의 가중치 2
};

// ===== 정점(Vertex) 구조체 =====
// 시·군 하나를 표현: 도시 이름 + 지도상 위치
struct City {
    QString name;  // 도시 이름 (예: "전주")
    QPointF pos;   // 지도 위에 노드를 그릴 좌표
};

// ===== 그래프 클래스 =====
// 도시(정점)들과 버스 노선(간선)들을 저장·관리하는 자료구조
class Graph {
public:
    // 새 도시(정점)를 추가하고, 그 도시의 인덱스 번호를 반환
    int addCity(const QString& name, QPointF pos) {
        m_cities.push_back({name, pos});   // 도시 정보를 정점 리스트에 추가
        m_adj.push_back({});               // 이 도시의 인접 리스트(빈 칸) 생성
        return m_cities.size() - 1;        // 방금 추가한 도시의 인덱스 반환
    }

    // 두 도시 u, v를 잇는 버스 노선(간선) 추가
    // 무방향 그래프이므로 양쪽 도시 모두에 노선을 등록
    void addRoute(int u, int v, int minutes, int fare) {
        m_adj[u].push_back({v, minutes, fare});  // u → v 방향 노선 등록
        m_adj[v].push_back({u, minutes, fare});  // v → u 방향 노선 등록
    }

    // 전체 도시 리스트를 읽기 전용으로 반환 (외부에서 조회용)
    const QVector<City>& cities() const { return m_cities; }

    // 전체 인접 리스트를 읽기 전용으로 반환 (노선 조회·렌더링용)
    const QVector<QVector<Route>>& adj() const { return m_adj; }

private:
    QVector<City> m_cities;            // 정점 리스트: 모든 도시 정보 저장
    QVector<QVector<Route>> m_adj;     // 인접 리스트: m_adj[i] = i번 도시에서 출발하는 노선들
};

#endif  // 인클루드 가드 끝
