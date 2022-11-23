#pragma once

#include <map>

class Color {
public:
    std::map <char, int> mp;
    std::map <char, int> direction = {{'r', 1}, {'g', 1}, {'b', 1}};

    unsigned long toRGB() {
        return RGB(mp['r'], mp['g'], mp['b']);
    }

    Color() {
        srand(time(NULL));
        set_random();
    }

    Color (int _r, int _g, int _b) {
        mp['r'] = _r;
        mp['g'] = _g;
        mp['b'] = _b;
    }

    int r() {
        return mp['r'];
    }

    int g() {
        return mp['g'];
    }

    int b() {
        return mp['b'];
    }

    void set_random() {
        mp['r'] = rand() % 256;
        mp['g'] = rand() % 256;
        mp['b'] = rand() % 256;
    }

    void plus() {
        plus_letter('r');
        plus_letter('g');
        plus_letter('b');
    }

    void minus() {
        minus_letter('r');
        minus_letter('g');
        minus_letter('b');
    }
private:
    void plus_letter(char letter) {
        mp[letter] += direction[letter];
        check_dir(letter);
    }

    void minus_letter(char letter) {
        mp[letter] -= direction[letter];
        check_dir(letter);
    }
    
    void check_dir(char letter) {
        if (mp[letter] > 255) {
            mp[letter] -= 2;
            direction[letter] *= -1;
        } else if (mp[letter] < 0) {
            mp[letter] += 2;
            direction[letter] *= -1;
        }
    }
};

std::ostream& operator<<(std::ostream &os, Color color) {
    return os << color.r() << " " << color.g() << " " << color.b();
}

std::ostream& operator>>(std::istream &is, Color& color) {
    is >> color.mp['r'] >> color.mp['g'] >> color.mp['b'];
}

Color operator+=(Color& color, int d) {
    color.plus();
    return color;
}

Color operator-=(Color& color, int d) {
    color.minus();
    return color;
}

