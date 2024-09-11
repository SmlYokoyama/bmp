#include "BMP.hpp"

#include <cmath>
#include <iostream>

#define BMP_WIDTH 511
#define BMP_HEIGHT 511

#define BMP_PATH "../images/img.bmp"

#define N 15

inline const float ellipseX(
    const float y, 
    const float a, 
    const float b
) {
    return a*(sqrt(1.0f - y*y/b/b));
}

inline const float ellipseY(
    const float x, 
    const float a, 
    const float b
) {
    return b*(sqrt(1.0f - x*x/a/a));
}

void drawEllipse(
    BMP::Image& img,
    const BMP::Color& color, 
    const float centerX, 
    const float centerY, 
    const int a, 
    const int b
) {
    int centerXInt = (int)centerX;
    int centerYInt = (int)centerY;

    float aFloat = (float)a;
    float bFloat = (float)b;

    // input -> output
    //   X   ->   Y
    for(int x = 0; x < a; ++x) {
        float y = ellipseY((float)x, aFloat, bFloat);
        img.setColor(color, centerXInt + x, (int)(centerY + y));
        img.setColor(color, centerXInt + x, (int)(centerY - y));
        img.setColor(color, centerXInt - x, (int)(centerY + y));
        img.setColor(color, centerXInt - x, (int)(centerY - y));
    }

    // input -> output
    //   Y   ->   X
    for(int y = 0; y < b; ++y) {
        float x = ellipseX((float)y, aFloat, bFloat);
        img.setColor(color, (int)(centerX + x), centerYInt + y);
        img.setColor(color, (int)(centerX - x), centerYInt + y);
        img.setColor(color, (int)(centerX + x), centerYInt - y);
        img.setColor(color, (int)(centerX - x), centerYInt - y);
    }
}

int main() {
    BMP::Image img(BMP_WIDTH, BMP_HEIGHT);

    for(int i = 0; i < BMP_HEIGHT; ++i) {
        for(int j = 0; j < BMP_WIDTH; ++j) {
            img.setColor(0, 0, 0, j, i);
        }
    }

    BMP::Color color(0, 255, 0);
    
    for(double i = 0.0; i <= M_PI/2.0; i += M_PI/2/(double)N) {
        drawEllipse(
            img, 
            color, 
            (float)BMP_WIDTH/2.0f, 
            (float)BMP_HEIGHT/2.0f, 
            (int)((double)BMP_WIDTH*std::cos(i)/2.0), 
            BMP_HEIGHT/2
        );

        drawEllipse(
            img, 
            color, 
            (float)BMP_WIDTH/2.0f, 
            (float)BMP_HEIGHT/2.0f, 
            BMP_WIDTH/2, 
            (int)((double)BMP_HEIGHT*std::cos(i)/2.0)
        );
    }

    img.save(BMP_PATH);

    return 0;
}