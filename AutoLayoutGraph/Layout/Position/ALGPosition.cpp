//
//  ALGPosition.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#include "ALGPosition.hpp"
#include "../../Nodes/ALGNodeSection.hpp"

ALGPoint ALGPosition::originInSection(ALGNodeSection* section) {
    
    ALGPoint sectionOrigin = ALGPoint::zero;
    
    for (ALGNode* node : section->nodes) {
        sectionOrigin = ALGPoint(min(sectionOrigin.x, node->position.origin.x),
                                 min(sectionOrigin.y, node->position.origin.y));
    }
    
    return origin - sectionOrigin;
}
