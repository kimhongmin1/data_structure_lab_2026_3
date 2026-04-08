#include "Polynomial.h"

int main() {
    Polynomial a, b, c;

    a.read();          // 다항식 a 키보드 입력
    b.read();          // 다항식 b 키보드 입력
    c.add(a, b);       // c = a + b

    a.display("A = "); // 다항식 a 출력
    b.display("B = "); // 다항식 b 출력
    c.display("A+B="); // 다항식 c 출력

    return 0;
}
