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
#include "../Wires/ALGWire.hpp"
#include "../Helpers/Remove.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Equal.hpp"

ALGNode::ALGNode(string typeName)
: origin(ALGPoint::zero), typeName(typeName)
{
    uuid_generate(id);
}

class ALGNodeException : public runtime_error {
public:
    ALGNodeException(const string& message) : runtime_error(message) {}
};

// MARK: - Hit Test

bool ALGNode::hitTest(ALGPoint point, ALGLayout layout) {
    ALGSize size = this->size(layout);
    bool hitX = point.x >= 0 && point.x < size.width;
    bool hitY = point.y >= 0 && point.y < size.height;
    return hitX && hitY;
}

// MARK: - Connect

void ALGNode::connect(ALGNode* leadingNode, ALGNode* trailingNode)
{
    cout << "will connect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
    if (ALGNode::isConnected(leadingNode, trailingNode)) {
        throw ALGNodeException("Connection failed, already connected.");
    }
    if (ALGNode::isLoop(leadingNode, trailingNode)) {
        throw ALGNodeException("Connection failed, loop found.");
    }
    ALGWire* wire = new ALGWire(leadingNode, trailingNode);
    connect(wire);
    cout << "did connect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

void ALGNode::connect(ALGWire* wire)
{
    cout << "will connect wire" << endl;
    wire->leadingNode->outputWires.push_back(wire);
    wire->trailingNode->inputWires.push_back(wire);
    wire->commonParent()->updateSectionsOnDidConnect(wire);
    cout << "did connect wire" << endl;
}

// MARK: - Disconnect

void ALGNode::disconnect(ALGNode* leadingNode, ALGNode* trailingNode)
{
    cout << "will disconnect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
    if (!ALGNode::isConnected(leadingNode, trailingNode)) {
        throw ALGNodeException("Disconnect failed, already disconnected.");
    }
    ALGWire* wire = optionalWire(leadingNode, trailingNode);
    if (wire == nullptr) {
        throw ALGNodeException("Disconnect failed, wire not found.");
    }
    ALGNode::disconnect(wire);
    delete wire;
    cout << "did disconnect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

void ALGNode::disconnect(ALGWire* wire)
{
    cout << "will disconnect wire" << endl;
    removeIn(wire->leadingNode->outputWires, wire);
    removeIn(wire->trailingNode->inputWires, wire);
    wire->commonParent()->updateSectionsOnDidDisconnect(wire);
    cout << "did disconnect wire" << endl;
}

// MARK: - Wire

ALGWire* ALGNode::optionalWire(ALGNode* leadingNode, ALGNode* trailingNode) {
    for (ALGWire* wire : leadingNode->outputWires) {
        if (wire->trailingNode == trailingNode) {
            return wire;
        }
    }
    return nullptr;
}

// MARK: - Is

bool ALGNode::isConnected(ALGNode* leadingNode, ALGNode* trailingNode) {
    return ALGNode::optionalWire(leadingNode, trailingNode) != nullptr;
}

bool ALGNode::isLoop(ALGNode* leadingNode, ALGNode* trailingNode) {
    if (trailingNode->containsDownstream(leadingNode->id)) {
        return true;
    }
    if (leadingNode->containsUpstream(trailingNode->id)) {
        return true;
    }
    return false;
}

// MARK: - Stream

bool ALGNode::containsDownstream(uuid_t id)
{
    for (ALGWire* wire : outputWires) {
        if (wire->trailingNode->id == id) {
            return true;
        } else if (wire->trailingNode->containsDownstream(id)) {
            return true;
        }
    }
    return false;
}
bool ALGNode::containsUpstream(uuid_t id)
{
    for (ALGWire* wire : inputWires) {
        if (wire->leadingNode->id == id) {
            return true;
        } else if (wire->leadingNode->containsUpstream(id)) {
            return true;
        }
    }
    return false;
}

// MARK: - Remove

void ALGNode::removeFromParent() {
    if (parent == nullptr) {
        throw ALGNodeException("Remove from parent failed, no parent found.");
    }
    parent->remove(this);
}
