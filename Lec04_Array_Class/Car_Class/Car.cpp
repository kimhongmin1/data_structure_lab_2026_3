#include "Car.h"
#include "SportCar.h"

int main() {
    Car myCar(60, "Sedan", 1);
    myCar.display();
    myCar.speedUp();
    myCar.changeGear(3);
    myCar.display();
    myCar.whereAmI();

    printf("\n");

    SportCar mySportCar(100, "Ferrari", 2, false);
    mySportCar.display();

    mySportCar.setTurbo(true);
    mySportCar.speedUp();          // 터보 ON → +20
    mySportCar.changeGear(4);
    mySportCar.display();
    mySportCar.whereAmI();

    return 0;
}
