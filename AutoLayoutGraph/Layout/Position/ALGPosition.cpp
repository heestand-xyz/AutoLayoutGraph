//
//  ALGPosition.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#include "ALGPosition.hpp"
#include "../../Nodes/ALGNodeSection.hpp"

ALGPoint ALGPosition::origin(ALGNodeSection* section) {
    
    ALGPoint origin = ALGPoint::zero;
    
    for (ALGNode* node : section->nodes) {
        origin = ALGPoint(min(origin.x, node->position.point.x),
                          min(origin.y, node->position.point.y));
    }
    
    return point - origin;
}
