#include <cstdio>
#include <stack>
#include <deque>
#include "Location2D.h"
using namespace std;

#define MAZE_SIZE 6

char map[MAZE_SIZE][MAZE_SIZE] = {
    { '1', '1', '1', '1', '1', '1' },
    { 'e', '0', '1', '0', '0', '1' },
    { '1', '0', '0', '0', '1', '1' },
    { '1', '0', '1', '0', '1', '1' },
    { '1', '0', '1', '0', '0', 'x' },
    { '1', '1', '1', '1', '1', '1' }
};

bool isValidLoc(int r, int c) {
    if (r < 0 || c < 0 || r >= MAZE_SIZE || c >= MAZE_SIZE) return false;
    return map[r][c] == '0' || map[r][c] == 'x';
}

int main() {
    int ds_num;
    printf("선택 : 1) Stack, 2) Deque, 3) Deque-BFS \n");
    scanf_s("%d", &ds_num);

    Location2D entry(1, 0);

    switch (ds_num) {
    case 1: { 
        stack<Location2D> locStack;
        locStack.push(entry);

        while (locStack.empty() == false) {
            Location2D here = locStack.top();
            locStack.pop();

            int r = here.row, c = here.col;
            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("\n미로 탐색 성공 (stack-DFS)\n");
                return 0;
            }
            else {
                map[r][c] = '.';
                if (isValidLoc(r - 1, c)) locStack.push(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locStack.push(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locStack.push(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locStack.push(Location2D(r, c + 1));
            }
        }
        printf("\n미로 탐색 실패\n");
        break;
    }
    case 2: {
        deque<Location2D> locDeque;
        locDeque.push_front(entry);

        while (locDeque.empty() == false) {
            Location2D here = locDeque.front();
            locDeque.pop_front();

            int r = here.row, c = here.col;
            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("\n미로 탐색 성공 (deque-DFS)\n");
                return 0;
            }
            else {
                map[r][c] = '.';
                if (isValidLoc(r - 1, c)) locDeque.push_front(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locDeque.push_front(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locDeque.push_front(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locDeque.push_front(Location2D(r, c + 1));
            }
        }
        printf("\n미로 탐색 실패\n");
        break;
    }
    case 3: {
        deque<Location2D> locDeque;
        locDeque.push_back(entry);

        while (locDeque.empty() == false) {
            Location2D here = locDeque.front();
            locDeque.pop_front();

            int r = here.row, c = here.col;
            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("\n미로 탐색 성공 (deque-BFS)\n");
                return 0;
            }
            else {
                map[r][c] = '.';
                if (isValidLoc(r - 1, c)) locDeque.push_back(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locDeque.push_back(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locDeque.push_back(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locDeque.push_back(Location2D(r, c + 1));
            }
        }
        printf("\n미로 탐색 실패\n");
        break;
    }
    default:
        printf("잘못된 선택입니다.\n");
        break;
    }
    return 0;
}
