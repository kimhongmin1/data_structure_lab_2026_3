#ifndef MAINWINDOW_H        // 헤더 중복 포함 방지 (인클루드 가드)
#define MAINWINDOW_H
#include <QMainWindow>      // 메인 창의 기반 클래스
#include <QWidget>          // 모든 위젯의 기본 클래스
#include <QHBoxLayout>      // 가로 배치 레이아웃
#include <QVBoxLayout>      // 세로 배치 레이아웃
#include <QLabel>           // 텍스트/제목 표시용 위젯
#include <QPushButton>      // 예매 버튼
#include <QScrollArea>      // 노선이 많을 때 스크롤되는 영역
#include <QFrame>           // 노선 한 개를 담는 카드 틀
#include <QMessageBox>      // "예매 완료" 팝업 창
#include <QPainter>         // 지도/노드를 직접 그리는 도구
#include <QPainterPath>     // 곡선 경로 그리기용 (배경 등)
#include <QPixmap>          // 배경 지도 이미지 저장 클래스
#include <QMouseEvent>      // 마우스 클릭/이동 이벤트 정보
#include <functional>       // std::function (콜백 함수 저장)
#include "graph.h"          // 앞서 만든 그래프 자료구조

// ============================================================
// 지도 위젯: 배경 지도 + 도시 노드를 그리고, 클릭을 감지하는 화면
// ============================================================
class MapWidget : public QWidget {     // QWidget을 상속해 그리기/이벤트 처리
public:
    // 생성자: 그래프 포인터를 받아 저장
    MapWidget(Graph* g, QWidget* parent=nullptr) : QWidget(parent), m_graph(g) {
        setMouseTracking(true);   // 버튼 안 눌러도 마우스 이동 감지(hover용)
        setMinimumWidth(440);     // 지도 영역 최소 너비 보장
        m_bg.load("citymap.png"); // 배경 지도 이미지 로드 (없으면 빈 Pixmap)
    }
    int selected() const { return m_sel; }   // 현재 선택된 도시 인덱스 반환

    // 도시가 클릭됐을 때 외부(MainWindow)로 알려주는 콜백 함수
    std::function<void(int)> onCityClicked;

protected:
    // ----- 마우스가 움직일 때 호출 -----
    void mouseMoveEvent(QMouseEvent* e) override {
        int h = cityAt(e->pos());            // 마우스 위치에 도시가 있는지 검사
        if (h != m_hover) { m_hover = h; update(); } // 바뀌면 강조 갱신
    }
    // ----- 마우스 버튼을 눌렀을 때 호출 -----
    void mousePressEvent(QMouseEvent* e) override {
        int h = cityAt(e->pos());            // 클릭 지점의 도시 찾기
        if (h < 0) return;                   // 도시가 아니면 무시
        m_sel = h;                           // 선택 도시 갱신
        if (onCityClicked) onCityClicked(h); // 콜백으로 MainWindow에 알림
        update();                            // 화면 다시 그리기
    }
    // ----- 화면을 그릴 때마다 호출(핵심 렌더링) -----
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);                          // 이 위젯에 그릴 화가 객체
        p.setRenderHint(QPainter::Antialiasing);   // 부드러운 곡선(안티앨리어싱)

        // 배경: 지도 이미지가 있으면 깔고, 없으면 단색으로 채움
        if (!m_bg.isNull()) p.drawPixmap(rect(), m_bg);
        else p.fillRect(rect(), QColor(240,245,240));

        const auto& c = m_graph->cities();   // 도시 목록 읽기(읽기 전용)

        // 1) 모든 노선(간선) 회색 선으로 그리기
        p.setPen(QPen(QColor(140,150,160), 2));
        for (int u=0; u<c.size(); ++u)               // 모든 도시에 대해
            for (const Route& r : m_graph->adj()[u]) // 그 도시의 노선들 순회
                if (u < r.to)                        // 중복 그리기 방지(u<v만)
                    p.drawLine(c[u].pos, c[r.to].pos);

        // 2) 선택된 도시에서 나가는 노선만 주황색으로 강조
        if (m_sel >= 0) {
            p.setPen(QPen(QColor(230,90,40), 4));
            for (const Route& r : m_graph->adj()[m_sel])
                p.drawLine(c[m_sel].pos, c[r.to].pos);
        }

        // 3) 도시 노드(원)와 이름 그리기
        for (int i=0; i<c.size(); ++i) {
            QColor col(255,255,255);                 // 기본은 흰색
            if (i == m_sel) col = QColor(230,90,40);  // 선택 도시는 주황
            else if (i == m_hover) col = QColor(255,224,178); // hover는 연주황
            p.setBrush(col);                          // 원 채우기 색
            p.setPen(QPen(Qt::black, 2));             // 원 테두리 검정
            int rad = (i==m_hover||i==m_sel) ? 11 : 8; // 강조 시 더 큰 원
            p.drawEllipse(c[i].pos, rad, rad);        // 도시 노드 원 그리기

            // 도시 이름: 흰 외곽선을 먼저 8방향으로 그려 가독성 확보
            p.setPen(Qt::white);
            for (int dx=-1; dx<=1; ++dx) for (int dy=-1; dy<=1; ++dy)
                p.drawText(c[i].pos + QPointF(13+dx, 4+dy), c[i].name);
            p.setPen(Qt::black);                      // 그 위에 검은 글자
            p.drawText(c[i].pos + QPointF(13, 4), c[i].name);
        }
    }

private:
    // 주어진 화면 좌표에 가까운 도시 인덱스를 반환(없으면 -1)
    int cityAt(const QPoint& pt) {
        const auto& c = m_graph->cities();
        for (int i=0; i<c.size(); ++i) {
            QPointF d = c[i].pos - QPointF(pt);       // 도시와 클릭점의 차이
            // 거리의 제곱이 14px 반경 이내면 그 도시를 클릭한 것으로 판정
            if (d.x()*d.x() + d.y()*d.y() <= 14*14) return i;
        }
        return -1;                                    // 어떤 도시도 아님
    }
    Graph* m_graph;          // 그래프 데이터(소유 아님, 참조만)
    QPixmap m_bg;            // 배경 지도 이미지
    int m_sel=-1, m_hover=-1; // 선택/마우스오버 중인 도시 인덱스(-1=없음)
};

// ============================================================
// 메인 윈도우: 왼쪽 지도 + 오른쪽 노선/예매 패널을 합친 전체 창
// ============================================================
class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent=nullptr) : QMainWindow(parent) {
        setWindowTitle("전라북도 시·군 버스 노선 예매"); // 창 제목
        resize(900, 660);                              // 창 크기
        buildData();                                   // 도시·노선 데이터 구성

        auto* central = new QWidget(this);   // 중앙에 놓을 컨테이너 위젯
        setCentralWidget(central);
        auto* root = new QHBoxLayout(central); // 좌우 가로 분할 레이아웃
        root->setContentsMargins(0,0,0,0);     // 바깥 여백 제거
        root->setSpacing(0);                   // 위젯 사이 간격 제거

        // 왼쪽: 지도 위젯 생성 + 클릭 콜백 연결
        m_map = new MapWidget(&m_graph, this);
        m_map->onCityClicked = [this](int c){ showRoutes(c); }; // 클릭→노선표시
        root->addWidget(m_map, 3);             // 비율 3 (넓게)

        // 오른쪽: 노선/예매 패널
        auto* side = new QWidget(this);
        side->setFixedWidth(320);              // 패널 고정 너비
        side->setStyleSheet("background:#2b3a4a;"); // 짙은 남색 배경
        auto* sv = new QVBoxLayout(side);      // 패널 내부 세로 배치

        m_title = new QLabel("도시를 클릭하세요"); // 상단 제목 라벨
        m_title->setStyleSheet("color:white;font-size:16px;font-weight:bold;padding:6px;");
        sv->addWidget(m_title);

        // 노선 카드가 쌓일 스크롤 영역
        m_scroll = new QScrollArea(this);
        m_scroll->setWidgetResizable(true);    // 내용에 맞춰 크기 조정
        m_scroll->setStyleSheet("border:none;background:#2b3a4a;");
        m_listHost = new QWidget();            // 스크롤 안에 들어갈 실제 위젯
        m_listHost->setStyleSheet("background:#2b3a4a;");
        m_listLayout = new QVBoxLayout(m_listHost); // 카드들을 세로로 쌓음
        m_listLayout->addStretch();            // 아래쪽 빈 공간(카드 위로 정렬)
        m_scroll->setWidget(m_listHost);
        sv->addWidget(m_scroll, 1);

        root->addWidget(side, 0);              // 오른쪽 패널은 고정폭(비율 0)
    }

private:
    // ----- 도시(정점)와 노선(간선) 데이터를 그래프에 입력 -----
    void buildData() {
        // 배경 지도 지명 위치에 맞춘 14개 시·군 좌표
        int gunsan = m_graph.addCity("군산", {130, 220});  // 북서 해안
        int iksan  = m_graph.addCity("익산", {225, 190});  // 북부
        int wanju  = m_graph.addCity("완주", {320, 210});  // 북중부
        int muju   = m_graph.addCity("무주", {480, 250});  // 북동 끝
        int gimje  = m_graph.addCity("김제", {200, 280});  // 서부
        int jeonju = m_graph.addCity("전주", {270, 280});  // 중앙
        int jinan  = m_graph.addCity("진안", {370, 275});  // 동중부
        int buan   = m_graph.addCity("부안", {120, 330});  // 남서 해안
        int jeongeup=m_graph.addCity("정읍", {200, 365});  // 남서부
        int imsil  = m_graph.addCity("임실", {310, 380});  // 남중부
        int jangsu = m_graph.addCity("장수", {420, 360});  // 남동부
        int gochang= m_graph.addCity("고창", {100, 430});  // 남서 끝
        int sunchang=m_graph.addCity("순창", {270, 460});  // 남부
        int namwon = m_graph.addCity("남원", {380, 470});  // 최남단

        // 시·군을 잇는 15개 버스 노선 (소요시간 분, 요금 원)
        m_graph.addRoute(gunsan, iksan,   30, 2800);
        m_graph.addRoute(gunsan, gimje,   35, 3200);
        m_graph.addRoute(iksan,  wanju,   25, 2600);
        m_graph.addRoute(iksan,  jeonju,  40, 3500);
        m_graph.addRoute(wanju,  jeonju,  20, 2400);
        m_graph.addRoute(wanju,  jinan,   45, 4200);
        m_graph.addRoute(jinan,  muju,    40, 4000);
        m_graph.addRoute(jinan,  jangsu,  35, 3600);
        m_graph.addRoute(gimje,  jeonju,  30, 3000);
        m_graph.addRoute(gimje,  buan,    35, 3300);
        m_graph.addRoute(buan,   jeongeup,40, 3800);
        m_graph.addRoute(jeonju, imsil,   45, 4300);
        m_graph.addRoute(jeongeup,sunchang,50,4600);
        m_graph.addRoute(imsil,  namwon,  40, 3900);
        m_graph.addRoute(jangsu, namwon,  55, 5000);
    }

    // ----- 우측 패널의 기존 노선 카드를 모두 삭제 -----
    void clearList() {
        QLayoutItem* it;
        // 레이아웃에서 항목을 하나씩 꺼내며 위젯과 항목을 메모리 해제
        while ((it = m_listLayout->takeAt(0)) != nullptr) {
            if (it->widget()) it->widget()->deleteLater(); // 위젯 안전 삭제
            delete it;                                     // 레이아웃 항목 삭제
        }
    }

    // ----- 선택한 도시의 노선들을 우측 패널에 카드로 표시 -----
    void showRoutes(int cityIdx) {
        const auto& cities = m_graph.cities();
        m_title->setText(cities[cityIdx].name + " 출발 노선"); // 제목 갱신
        clearList();                                           // 이전 목록 비우기

        // 선택 도시의 인접 리스트(=출발 노선)를 순회하며 카드 생성
        for (const Route& r : m_graph.adj()[cityIdx]) {
            QString dep = cities[cityIdx].name;   // 출발 도시 이름
            QString arr = cities[r.to].name;      // 도착 도시 이름

            auto* card = new QFrame();            // 노선 한 개를 담는 카드
            card->setStyleSheet(
                "QFrame{background:#3c4f63;border-radius:8px;}"
                "QLabel{color:white;}");
            auto* cv = new QVBoxLayout(card);     // 카드 내부 세로 배치

            auto* line = new QLabel(dep + "  →  " + arr); // 출발→도착
            line->setStyleSheet("font-size:15px;font-weight:bold;color:white;");
            auto* info = new QLabel(              // 소요시간·요금 정보
                QString("소요 %1분 · 요금 %2원").arg(r.minutes).arg(r.fare));
            info->setStyleSheet("color:#cdd7e0;font-size:12px;");

            auto* btn = new QPushButton("예매");   // 예매 버튼
            btn->setStyleSheet(
                "QPushButton{background:#ff7043;color:white;border:none;"
                "padding:6px;border-radius:6px;font-weight:bold;}"
                "QPushButton:hover{background:#ff8a65;}");

            int fare = r.fare;  // 람다에 값으로 캡처하기 위해 복사
            // 버튼 클릭 시그널 → 람다 슬롯 연결: 예매 완료 팝업 출력
            connect(btn, &QPushButton::clicked, this, [this,dep,arr,fare](){
                QMessageBox::information(this, "예매 완료",
                    dep + " → " + arr + " 노선 예매가 완료되었습니다.\n결제 금액: "
                    + QString::number(fare) + "원");
            });

            cv->addWidget(line);                 // 카드에 위젯 3개 추가
            cv->addWidget(info);
            cv->addWidget(btn);
            m_listLayout->addWidget(card);       // 완성된 카드를 패널에 추가
        }
        m_listLayout->addStretch();              // 카드들을 위쪽으로 정렬
    }

    Graph m_graph;            // 도시·노선 데이터(그래프) — MainWindow가 소유
    MapWidget* m_map;         // 왼쪽 지도 위젯
    QLabel* m_title;          // 우측 패널 제목
    QScrollArea* m_scroll;    // 노선 카드 스크롤 영역
    QWidget* m_listHost;      // 스크롤 내부 컨테이너
    QVBoxLayout* m_listLayout;// 노선 카드들을 쌓는 레이아웃
};
#endif  // 인클루드 가드 끝
