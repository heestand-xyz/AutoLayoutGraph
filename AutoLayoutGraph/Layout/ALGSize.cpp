//
//  ALGSize.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGSize.hpp"

ALGSize::ALGSize(int width, int height) {
    this->width = width;
    this->height = height;
}

const ALGSize ALGSize::zero(0, 0);
const ALGSize ALGSize::one(1, 1);

ALGSize ALGSize::padding(int padding) {
    return ALGSize(width, height) + padding * 2;
}

ALGSize ALGSize::operator+(ALGSize size) {
    return ALGSize(width + size.width, height + size.height);
}
ALGSize ALGSize::operator-(ALGSize size) {
    return ALGSize(width - size.width, height - size.height);
}
ALGSize ALGSize::operator*(ALGSize size) {
    return ALGSize(width * size.width, height * size.height);
}
ALGSize ALGSize::operator/(ALGSize size) {
    return ALGSize(width / size.width, height / size.height);
}

ALGSize ALGSize::operator+(int value) {
    return ALGSize(width + value, height + value);
}
ALGSize ALGSize::operator-(int value) {
    return ALGSize(width - value, height - value);
}
ALGSize ALGSize::operator*(int value) {
    return ALGSize(width * value, height * value);
}
ALGSize ALGSize::operator/(int value) {
    return ALGSize(width / value, height / value);
}
