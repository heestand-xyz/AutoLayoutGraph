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
