//
// Created by PC on 09.11.2022.
//

#include <iostream>
#include <tchar.h>
#include <windowsx.h>
#include <cstdint>
#include <fstream>
#include <windows.h>

class Color {
public:
    int r, g, b;

    unsigned long toRGB() {
        return RGB(r, g, b);
    }

    Color() {
        srand( time(nullptr) * 100);
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    }

    Color (int _r, int _g, int _b) {
        r = _r;
        g = _g;
        b = _b;
    }
};

Color operator+(Color color_1, Color color_2) {
    return Color{color_1.r + color_2.r, color_1.g + color_2.g, color_1.b + color_2.b};
}

std::ostream& operator<<(std::ostream &os, const Color& color) {
    return os  << '(' << color.r << ", " << color.g << ", " << color.b << ")";
}
