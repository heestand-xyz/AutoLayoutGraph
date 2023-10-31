//
//  ALGSize.cpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#include "ALGSize.hpp"

ALGSize::ALGSize(int width, int height) {
    this->width = width;
    this->height = height;
}

const ALGSize ALGSize::zero(0, 0);
const ALGSize ALGSize::one(1, 1);
