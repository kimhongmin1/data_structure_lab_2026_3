#include <stdio.h>
#define	HEIGHT	8
#define	WIDTH	8

int image[HEIGHT][WIDTH] = {
  {  10,  30,  55,  80, 120, 160, 200, 230},
  {  20,  45,  70, 100, 140, 180, 220, 210},
  {  35,  60,  90, 130, 170, 210, 240, 190},
  {  50,  80, 115, 150, 190, 230, 255, 170},
  {  40,  65, 100, 140, 175, 215, 235, 150},
  {  25,  50,  80, 115, 155, 195, 210, 130},
  {  15,  35,  60,  90, 130, 165, 185, 110},
  {    5,  20,  40,  70, 105, 140, 160,  90},
};
int getMaxPixel(int img[][WIDTH], int height) {
    int maxVal = img[0][0];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < WIDTH; j++)
            if (img[i][j] > maxVal)
                maxVal = img[i][j];
    return maxVal;
}

int main() {
    int maxBrightness = getMaxPixel(image, HEIGHT);
    printf("최대 화소 밝기값 = %d\n", maxBrightness);
    return 0;
}