#pragma once
#include "Car.h"

class SportCar : public Car {
public:
    bool bTurbo;

    SportCar(int s, const char* n, int g, bool t)
        : Car(s, n, g), bTurbo(t) {
    }    // ✅ 이 생성자가 없어서 오류 발생

    void setTurbo(bool bTur) { bTurbo = bTur; }
    void speedUp() {
		if (bTurbo) speed += 20, printf("터보 ON\n");
        else Car::speedUp();
    }
};
