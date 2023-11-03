//
//  ALGNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <iostream>
#include <stdexcept>
#include "ALGNode.hpp"
#include "ALGPoint.hpp"
#include "ALGGroupNode.hpp"
#include "ALGNodeSection.hpp"
#include "../Wires/ALGWire.hpp"
#include "../Helpers/Remove.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Append.hpp"

ALGNode::ALGNode(string typeName)
: typeName(typeName), position(ALGPosition())
{}

// MARK: - Exception

class ALGNodeException : public runtime_error {
public:
    ALGNodeException(const string& message) : runtime_error(message) {}
};

// MARK: - Hit Test

bool ALGNode::hitTest(ALGPoint point, ALGLayout layout) {
    ALGPoint origin = this->position.originInSection(section());
    ALGSize size = this->size(layout);
    bool hitX = point.x >= origin.x && point.x < origin.x + size.width;
    bool hitY = point.y >= origin.y && point.y < origin.y + size.height;
    return hitX && hitY;
}

// MARK: - Stream

bool ALGNode::containsDownstream(ALGNode* node)
{
    for (ALGWire* wire : outputWires) {
        if (wire->trailingNode == node) {
            return true;
        } else if (wire->trailingNode->containsDownstream(node)) {
            return true;
        }
    }
    return false;
}
bool ALGNode::containsUpstream(ALGNode* node)
{
    for (ALGWire* wire : inputWires) {
        if (wire->leadingNode == node) {
            return true;
        } else if (wire->leadingNode->containsUpstream(node)) {
            return true;
        }
    }
    return false;
}

// MARK: - Root

ALGGroupNode* ALGNode::root() {
    if (parent == nullptr) {
        return dynamic_cast<ALGGroupNode*>(this);
    }
    ALGGroupNode* currentParent = parent;
    while (!currentParent->isRoot()) {
        currentParent = currentParent->parent;
    }
    return currentParent;
}

// MARK: - Section

ALGNodeSection* ALGNode::section() {
    if (parent) {
        for (ALGNodeSection* section : parent->sections) {
            if (section->contains(this)) {
                return section;
            }
        }
    }
    return nullptr;
}

// MARK: - Auto Layout

void ALGNode::autoLayout(ALGLayout layout) {
    cout << "will auto layout node: " << typeName << endl;
    if (position.state == ALGPositionState::NONE) {
        position.origin = ALGPoint::zero;
        position.state = ALGPositionState::AUTO;
        bool didContinue = false;
        for (ALGWire* wire : inputWires) {
            ALGNode* node = wire->leadingNode;
            if (node->position.state == ALGPositionState::NONE) {
                node->autoLayout(wire, layout);
                didContinue = true;
                break;
            }
        }
        if (!didContinue) {
            for (ALGWire* wire : outputWires) {
                ALGNode* node = wire->trailingNode;
                if (node->position.state == ALGPositionState::NONE) {
                    node->autoLayout(wire, layout);
                    didContinue = true;
                    break;
                }
            }
        }
    } else if (position.state == ALGPositionState::AUTO) {
        
    }
    cout << "did auto layout node: " << typeName << endl;
}

void ALGNode::autoLayout(ALGWire* wire, ALGLayout layout) {
    cout << "will auto layout via wire node: " << typeName << endl;
    int originX = 0;
    for (ALGWire* inputWire : inputWires) {
        ALGNode* node = inputWire->leadingNode;
        if (node->position.state != ALGPositionState::NONE) {
            originX = node->position.origin.x + node->size(layout).width + layout.spacing;
            // ...
        }
    }
    for (ALGWire* outputWire : outputWires) {
        // ...
    }
    cout << "did auto layout via wire node: " << typeName << endl;
}
