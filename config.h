#include <fstream>
#include <iostream>

#include "color.h"

void read_config(int& cells_num, int& width, int& height, Color& background_color) {
    freopen("config.txt", "r", stdin);
    if (std::cin) {
        std::cin >> cells_num >> width >> height >> background_color;
        std::cout << "cn = " << cells_num << std::endl;
    }

}
