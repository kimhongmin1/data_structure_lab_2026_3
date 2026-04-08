#pragma once
#include <stdio.h>
#include <string.h>

class Car {
protected:
    int speed;
    char name[40];
public:
    int gear;

    Car() : speed(0), gear(1) {
        strcpy_s(name, "noname");
    }
    Car(int s, const char* n, int g)
        : speed(s), gear(g) {
        strcpy_s(name, n);
    }

    void changeGear(int g = 4) {
        gear = g;
    }
    void speedUp() {
        speed += 5;
    }
    void display() {
        printf(" [%s] : 기어 = %d단 속도 = %dkm/h\n", name, gear, speed);
    }
    void whereAmI() {
        printf("객체주소 = %p\n", this);
    }
};
