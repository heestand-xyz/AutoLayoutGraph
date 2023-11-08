//
//  ALGSize.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGSize.hpp"

ALGSize::ALGSize(double width, double height) {
    this->width = width;
    this->height = height;
}

const ALGSize ALGSize::zero(0, 0);
const ALGSize ALGSize::one(1, 1);

ALGSize ALGSize::padding(double padding) {
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

ALGSize ALGSize::operator+(double value) {
    return ALGSize(width + value, height + value);
}
ALGSize ALGSize::operator-(double value) {
    return ALGSize(width - value, height - value);
}
ALGSize ALGSize::operator*(double value) {
    return ALGSize(width * value, height * value);
}
ALGSize ALGSize::operator/(double value) {
    return ALGSize(width / value, height / value);
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGSize& size) {
    os << "size(width: " << size.width << ", height: " << size.height << ")";
    return os;
}

string ALGSize::description() {
    return "size(width: " + to_string(width) + ", height: " + to_string(height) + ")";
}
