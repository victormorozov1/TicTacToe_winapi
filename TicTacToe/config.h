#pragma once

#include <fstream>
#include <iostream>

#include "color.h"

void read_config(int& cells_num, int& width, int& height, Color& background_color) {
    std::ifstream fin;
    fin.open("config.txt");
    if (fin) {
        fin >> cells_num >> width >> height >> background_color;
    }
}

void write_config(int cells_num, int width, int height, Color background_color) {
    std::cout << cells_num << " " << width << " " << height << " " << background_color;
    std::ofstream fout;
    fout.open("config.txt");
    fout << cells_num << " " << width << " " << height << " " << background_color;
}
