//
//  ALGNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <os/log.h>
#include <stdexcept>
#include "ALGNode.hpp"
#include "ALGGroupNode.hpp"
#include "ALGNodeSection.hpp"
#include "../Layout/Types/ALGPoint.hpp"
#include "../Wires/ALGWire.hpp"
#include "../Helpers/Remove.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Append.hpp"

ALGNode::ALGNode(string typeName)
: typeName(typeName), position(ALGPosition())
{}

// MARK: - Logger

os_log_t nodeLogger = os_log_create("AutoLayoutGraph", "ALGNode");

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

// MARK: - Wires

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

vector<ALGWire*> ALGNode::inputWiresWithCommonParent() {
    vector<ALGWire*> groupWires = vector<ALGWire*>();
    for (ALGWire* inputWire : inputWires) {
        if (inputWire->hasCommonParent()) {
            groupWires.push_back(inputWire);
        }
    }
    return groupWires;
}

vector<ALGWire*> ALGNode::outputWiresWithCommonParent() {
    vector<ALGWire*> groupWires = vector<ALGWire*>();
    for (ALGWire* outputWire : outputWires) {
        if (outputWire->hasCommonParent()) {
            groupWires.push_back(outputWire);
        }
    }
    return groupWires;
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
    os_log_fault(nodeLogger, "parent not found for section");
    throw ALGNodeException("parent not found for section");
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGNode* node) {
    os << "node('" << node->typeName << "')";
    return os;
}

string ALGNode::description() {
    return "node('" + typeName + "')";
}
