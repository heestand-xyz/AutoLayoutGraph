//
//  ALGWire.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-01.
//

#include "ALGWire.hpp"
#include "../Nodes/ALGGroupNode.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Append.hpp"

ALGWire::ALGWire(ALGNode* leadingNode, ALGNode* trailingNode) :
leadingNode(leadingNode),
trailingNode(trailingNode)
{ }

bool ALGWire::hasCommonParent() {
    return leadingNode->parent == trailingNode->parent;
}

ALGGroupNode* ALGWire::commonParent() {
    return leadingNodeWithCommonParent()->parent;
}

ALGNode* ALGWire::leadingNodeWithCommonParent() {
    ALGNode* currentNode = leadingNode;
    while (!currentNode->parent->deepContains(trailingNode)) {
        currentNode = currentNode->parent;
    }
    return currentNode;
}

ALGNode* ALGWire::trailingNodeWithCommonParent() {
    ALGNode* currentNode = trailingNode;
    while (!currentNode->parent->deepContains(leadingNode)) {
        currentNode = currentNode->parent;
    }
    return currentNode;
}

bool ALGWire::isIndirectlyConnected() {
    return isIndirectlyConnectedWith(this, leadingNode, vector<ALGNode*>());
}

bool ALGWire::isIndirectlyConnectedWith(ALGWire* wire, ALGNode* checkNode, vector<ALGNode*> checkedNodes) {
    checkedNodes.push_back(checkNode);
    for (ALGWire* inputWire : checkNode->inputWires) {
        if (inputWire == wire) {
            continue;
        }
        if (inputWire->leadingNode == wire->leadingNode || inputWire->leadingNode == wire->trailingNode) {
            return true;
        }
        if (containsWhere(checkedNodes, [inputWire](ALGNode* node) {
            return inputWire->leadingNode == node;
        })) {
            continue;
        }
        if (isIndirectlyConnectedWith(wire, inputWire->leadingNode, checkedNodes)) {
            return true;
        }
    }
    for (ALGWire* outputWire : checkNode->outputWires) {
        if (outputWire == wire) {
            continue;
        }
        if (outputWire->trailingNode == wire->leadingNode || outputWire->trailingNode == wire->trailingNode) {
            return true;
        }
        if (containsWhere(checkedNodes, [outputWire](ALGNode* node) {
            return outputWire->trailingNode == node;
        })) {
            continue;
        }
        if (isIndirectlyConnectedWith(wire, outputWire->trailingNode, checkedNodes)) {
            return true;
        }
    }
    return false;
}

vector<ALGNode*> ALGWire::leadingNodes() {
    return nodesWith(this, leadingNode, vector<ALGNode*>());
}

vector<ALGNode*> ALGWire::trailingNodes() {
    return nodesWith(this, trailingNode, vector<ALGNode*>());
}

vector<ALGNode*> ALGWire::nodesWith(ALGWire* wire, ALGNode* checkNode, vector<ALGNode*> checkedNodes) {
    checkedNodes.push_back(checkNode);
    for (ALGWire* inputWire : checkNode->inputWires) {
        if (inputWire == wire) {
            continue;
        }
        if (inputWire->leadingNode == wire->leadingNode || inputWire->leadingNode == wire->trailingNode) {
            return checkedNodes;
        }
        if (containsWhere(checkedNodes, [inputWire](ALGNode* node) {
            return inputWire->leadingNode == node;
        })) {
            continue;
        }
        checkedNodes += nodesWith(inputWire, inputWire->leadingNode, checkedNodes);
    }
    return checkedNodes;
}

// MARK: - Auto Layout

void ALGWire::autoLayout(ALGLayout layout) {
    cout << "will auto layout: " << this << endl;
    if (leadingNode->position.state == ALGPositionState::FINAL) {
        autoRearrange(layout);
        return;
    }
    ALGPoint offset = autoOffset(layout);
    ALGPoint origin = trailingNode->position.origin + offset;
    leadingNode->position.origin = origin;
    if (trailingNode->position.state == ALGPositionState::FINAL) {
        leadingNode->position.state = ALGPositionState::FINAL;
    } else {
        leadingNode->position.state = ALGPositionState::AUTO;
    }
    for (ALGWire* inputWire : leadingNode->inputWires) {
        inputWire->autoLayout(layout);
    }
    cout << "did auto layout: " << this << endl;
}

void ALGWire::autoRearrange(ALGLayout layout) {
    cout << "will auto rearrange: " << this << endl;
    ALGPoint offset = autoOffset(layout);
    trailingNode->position.origin = leadingNode->position.origin - offset;
    trailingNode->position.state = ALGPositionState::FINAL;
    for (ALGWire* outputWire : trailingNode->outputWires) {
        outputWire->autoRearrange(layout);
    }
    cout << "did auto rearrange: " << this << endl;
}

ALGPoint ALGWire::autoOffset(ALGLayout layout) {
    double offsetX = -layout.spacing - leadingNode->size(layout).width;
    vector<ALGWire*> parallelWires = trailingNode->inputWires;
    auto parallelCount = parallelWires.size();
    double y = 0.0;
    double height = 0.0;
    for (int i = 0; i < parallelCount; i++) {
        ALGWire* inputWire = parallelWires[i];
        if (inputWire == this) {
            y = height;
        }
        height += inputWire->leadingNode->size(layout).height;
        if (i < parallelCount - 1) {
            height += layout.spacing;
        }
    }
    double offsetY = y - height / 2 + trailingNode->size(layout).height / 2;
    return ALGPoint(offsetX, offsetY);
}

// MARK: - Print

ostream& operator<<(ostream& os, const ALGWire* wire) {
    os << "wire(from: " << wire->leadingNode << " to: " << wire->trailingNode << ")";
    return os;
}
