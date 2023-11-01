//
//  ALGPoint.cpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#include "ALGPoint.hpp"

ALGPoint::ALGPoint(int x, int y) {
    this->x = x;
    this->y = y;
}

const ALGPoint ALGPoint::zero(0, 0);

ALGPoint ALGPoint::operator+(ALGPoint point) {
    return ALGPoint(x + point.x, y + point.y);
}
ALGPoint ALGPoint::operator-(ALGPoint point) {
    return ALGPoint(x - point.x, y - point.y);
}
ALGPoint ALGPoint::operator*(ALGPoint point) {
    return ALGPoint(x * point.x, y * point.y);
}
ALGPoint ALGPoint::operator/(ALGPoint point) {
    return ALGPoint(x / point.x, y / point.y);
}

ALGPoint ALGPoint::operator+(int value) {
    return ALGPoint(x + value, y + value);
}
ALGPoint ALGPoint::operator-(int value) {
    return ALGPoint(x - value, y - value);
}
ALGPoint ALGPoint::operator*(int value) {
    return ALGPoint(x * value, y * value);
}
ALGPoint ALGPoint::operator/(int value) {
    return ALGPoint(x / value, y / value);
}
