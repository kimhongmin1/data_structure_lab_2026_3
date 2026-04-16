#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>

const int MAX_STRING = 50;
const int MAX_STACK_SIZE = 20;

class student {
	int id;
	char name[MAX_STRING];
	char dept[MAX_STRING];
public:
	student(int i = 0, const char* n = "", const char* d = "") {
		set(i, n, d);
	}
	void set(int i, const char* n, const char* d) {
		id = i;
		strncpy(name, n, MAX_STRING - 1);
		name[MAX_STRING - 1] = '\0';
		strncpy(dept, d, MAX_STRING - 1);
		dept[MAX_STRING - 1] = '\0';
	}
	void display() const {
		printf("학번: %d, 이름: %s, 학과: %s\n", id, name, dept);
	}
};

class StudentStack {
	int top;
	student data[MAX_STACK_SIZE];
public:
	StudentStack() : top(-1) {}

	bool isEmpty() const {
		return top == -1;
	}
	bool isFull() const {
		return top == MAX_STACK_SIZE - 1;
	}
	void push(const student& s) {
		if (isFull()) {
			printf("스택이 가득 찼습니다.\n");
			return;
		}
		data[++top] = s;
	}
	student pop() {
		if (isEmpty()) {
			printf("스택이 비어 있습니다.\n");
			return student();
		}
		return data[top--];
	}
	void display() const {
		printf("[스택 항목의 수 = %2d]\n", top + 1);
		for (int i = 0; i <= top; i++) {
			data[i].display();
		}
		printf("\n");
	}
};
