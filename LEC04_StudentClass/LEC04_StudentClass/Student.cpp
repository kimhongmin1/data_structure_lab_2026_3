#include "StudentStack.h"

int main() {
	StudentStack stack;
	stack.push(student(20230001, "홍길동", "컴퓨터공학과"));
	stack.push(student(20230002, "김철수", "전자공학과"));
	stack.push(student(20230003, "이영희", "기계공학과"));
	stack.push(student(20230004, "박민수", "화학공학과"));
	stack.display();
	stack.pop();
	stack.display();
	return 0;
}
