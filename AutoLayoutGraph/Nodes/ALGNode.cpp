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
#include "../Helpers/Remove.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Equal.hpp"

ALGNode::ALGNode(string typeName)
: origin(ALGPoint::zero)
{
    uuid_generate(id);
    this->typeName = typeName;
}

class ConnectException : public std::runtime_error {
public:
    ConnectException(const std::string& message) : std::runtime_error(message) {}
};

void ALGNode::connect(ALGNode* leadingNode, ALGNode* trailingNode)
{
    cout << "will connect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
    if (ALGNode::isConnected(leadingNode, trailingNode)) {
        throw ConnectException("Connection failed, already connected.");
    }
    if (ALGNode::isLoop(leadingNode, trailingNode)) {
        throw ConnectException("Connection failed, loop found.");
    }
    ALGWire* wire = new ALGWire(leadingNode, trailingNode);
    leadingNode->outputWires.push_back(wire);
    trailingNode->inputWires.push_back(wire);
    cout << "did connect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

void ALGNode::disconnect(ALGNode* leadingNode, ALGNode* trailingNode)
{
    cout << "will disconnect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
    if (!ALGNode::isConnected(leadingNode, trailingNode)) {
        throw ConnectException("Disconnect failed, already disconnected.");
    }
    ALGWire* wire = optionalWire(leadingNode, trailingNode);
    if (wire == nullptr) {
        throw ConnectException("Disconnect failed, wire not found.");
    }
    ALGNode::disconnect(wire);
    cout << "did disconnect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

void ALGNode::disconnect(ALGWire* wire)
{
    cout << "will disconnect wire" << endl;
    removeIn(wire->leadingNode->outputWires, wire);
    removeIn(wire->trailingNode->inputWires, wire);
    delete wire;
    cout << "did disconnect wire" << endl;
}

ALGWire* ALGNode::optionalWire(ALGNode* leadingNode, ALGNode* trailingNode) {
    for (ALGWire* wire : leadingNode->outputWires) {
        if (wire->trailingNode == trailingNode) {
            return wire;
        }
    }
    return nullptr;
}

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
