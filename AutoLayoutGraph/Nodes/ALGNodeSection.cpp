//
//  ALGNodeSection.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGNodeSection.hpp"
#include "ALGRect.hpp"

ALGSize ALGNodeSection::getSize(ALGLayout layout) {

    if (nodes.empty()) {
        return ALGSize::zero;
    }

    ALGRect* totalFrame = nullptr;
    for (ALGNode* node : nodes) {
        ALGPoint origin = node->origin;
        ALGSize size = node->getSize(layout);
        if (totalFrame == nullptr) {
            totalFrame = new ALGRect(origin, size);
        } else {
            totalFrame->origin.x = min(totalFrame->origin.x, origin.x);
            totalFrame->origin.y = min(totalFrame->origin.y, origin.y);
            totalFrame->size.width = max(totalFrame->size.width, origin.x + size.width - totalFrame->origin.x);
            totalFrame->size.height = max(totalFrame->size.height, origin.y + size.height - totalFrame->origin.y);
        }
    }
    
    ALGSize totalSize = totalFrame->size;
    delete totalFrame;
    
    return totalSize;
}

void ALGNodeSection::autoLayout(ALGLayout layout) {

    for (ALGNode* node : nodes) {
        // TODO: Implement
    }
}
