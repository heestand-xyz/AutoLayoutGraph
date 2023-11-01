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
    leadingNode->outputNodes.push_back(trailingNode);
    trailingNode->inputNodes.push_back(leadingNode);
    cout << "did connect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

void ALGNode::disconnect(ALGNode* leadingNode, ALGNode* trailingNode)
{
    cout << "will disconnect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
    if (!ALGNode::isConnected(leadingNode, trailingNode)) {
        throw ConnectException("Disconnect failed, already disconnected.");
    }
    removeIn(leadingNode->outputNodes, trailingNode);
    removeIn(trailingNode->inputNodes, leadingNode);
    cout << "did disconnect leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

bool ALGNode::isConnected(ALGNode* leadingNode, ALGNode* trailingNode) {
    if (containsWhere(leadingNode->outputNodes, [trailingNode](ALGNode* node) {
        return isEqual(node->id, trailingNode->id);
    })) {
        return true;
    }
    if (containsWhere(trailingNode->inputNodes, [leadingNode](ALGNode* node) {
        return isEqual(node->id, leadingNode->id);
    })) {
        return true;
    }
    return false;
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
    for (ALGNode* node : outputNodes) {
        if (node->id == id) {
            return true;
        } else if (node->containsDownstream(id)) {
            return true;
        }
    }
    return false;
}
bool ALGNode::containsUpstream(uuid_t id)
{
    for (ALGNode* node : inputNodes) {
        if (node->id == id) {
            return true;
        } else if (node->containsUpstream(id)) {
            return true;
        }
    }
    return false;
}
