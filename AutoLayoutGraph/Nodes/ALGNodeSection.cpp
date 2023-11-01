//
//  ALGNodeSection.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGNodeSection.hpp"
#include "ALGGroupNode.hpp"
#include "ALGRect.hpp"

bool ALGNodeSection::contains(ALGNode* node) {
    for (ALGNode* n : nodes) {
        if (n == node) {
            return true;
        }
    }
    return false;
}

bool ALGNodeSection::deepContains(ALGNode* node) {
    if (contains(node)) {
        return true;
    }
    for (ALGNode* n : nodes) {
        ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(n);
        if (groupNode) {
            if (groupNode->deepContains(node)) {
                return true;
            }
        }
    }
    return false;
}

bool ALGNodeSection::deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout) {
    for (ALGNode* n : nodes) {
        if (n == node) {
            return node->hitTest(point, layout);
        } else {
            ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(n);
            if (groupNode) {
                if (groupNode->deepHitTest(node, point, layout)) {
                    return true;
                }
            }
        }
    }
    return false;
}

ALGPoint ALGNodeSection::getOrigin(ALGLayout layout) {
    // TODO: Implement
    return ALGPoint::zero;
}

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

vector<ALGNode*> ALGNodeSection::finalNodes() {
    vector<ALGNode*> finalNodes = vector<ALGNode*>();
    for (ALGNode* node : nodes) {
        if (node->outputWires.empty()) {
            finalNodes.push_back(node);
        }
    }
    return finalNodes;
}
