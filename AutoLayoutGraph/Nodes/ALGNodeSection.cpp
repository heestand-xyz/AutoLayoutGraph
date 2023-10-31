//
//  ALGNodeSection.cpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#include "ALGNodeSection.hpp"
#include "ALGRect.hpp"

ALGSize ALGNodeSection::getSize() {

    if (nodes.empty()) {
        return ALGSize(0, 0);
    }

    ALGRect* frame = nullptr;
    for(ALGNode& node : nodes) {
        ALGPoint origin = node.origin;
        ALGSize size = node.getSize();
        if (frame == nullptr) {
            frame = new ALGRect(origin, size);
        } else {
            frame->origin.x = min(frame->origin.x, origin.x);
            frame->origin.y = min(frame->origin.y, origin.y);
            frame->size.width = max(frame->size.width, origin.x + size.width - frame->origin.x);
            frame->size.height = max(frame->size.height, origin.y + size.height - frame->origin.y);
        }
    }
    
    ALGSize size = frame->size;
    delete frame;
    
    return size;
}

void ALGNodeSection::autoLayout(ALGLayout layout) {

    for(ALGNode& node : nodes) {
        
    }
}
