//
//  ALGWire.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-01.
//


#include <os/log.h>
#include "ALGWire.hpp"
#include "../Nodes/ALGGroupNode.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Append.hpp"

ALGWire::ALGWire(ALGNode* leadingNode, ALGNode* trailingNode) :
leadingNode(leadingNode),
trailingNode(trailingNode)
{ }

// MARK: - Logger

os_log_t wireLogger = os_log_create("AutoLayoutGraph", "ALGWire");

// MARK: - Common Parent

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

vector<ALGNode*> ALGWire::leadingNodesWithCommonParent() {
    return nodesWithCommonParent(this, leadingNode, vector<ALGNode*>());
}

vector<ALGNode*> ALGWire::trailingNodesWithCommonParent() {
    return nodesWithCommonParent(this, trailingNode, vector<ALGNode*>());
}

vector<ALGNode*> ALGWire::nodesWithCommonParent(ALGWire* wire, ALGNode* checkNode, vector<ALGNode*> checkedNodes) {
    checkedNodes.push_back(checkNode);
    for (ALGWire* inputWire : checkNode->inputWiresWithCommonParent()) {
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
        checkedNodes += nodesWithCommonParent(inputWire, inputWire->leadingNode, checkedNodes);
    }
    for (ALGWire* outputWire : checkNode->outputWiresWithCommonParent()) {
        if (outputWire == wire) {
            continue;
        }
        if (outputWire->trailingNode == wire->trailingNode || outputWire->trailingNode == wire->leadingNode) {
            return checkedNodes;
        }
        if (containsWhere(checkedNodes, [outputWire](ALGNode* node) {
            return outputWire->trailingNode == node;
        })) {
            continue;
        }
        checkedNodes += nodesWithCommonParent(outputWire, outputWire->trailingNode, checkedNodes);
    }
    return checkedNodes;
}

// MARK: - Indirectly Connected

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

// MARK: - Auto Layout

void ALGWire::autoLayout(ALGLayout layout) {
    os_log_info(wireLogger, "will auto layout: %{public}s",
                description().c_str());
    if (leadingNode->position.state == ALGPositionState::FINAL) {
        autoRearrange(layout);
        return;
    }
    ALGPoint offset = ALGPoint(-offsetX(layout), -offsetY(layout));
    ALGPoint origin = trailingNode->position.origin + offset;
    if (trailingNode->position.state == ALGPositionState::FINAL) {
        leadingNode->position.finalizeOrigin(origin);
    } else {
        leadingNode->position.temporaryOrigin(origin);
    }
    for (ALGWire* inputWire : leadingNode->inputWiresWithCommonParent()) {
        inputWire->autoLayout(layout);
    }
    os_log_info(wireLogger, "did auto layout: %{public}s",
                description().c_str());
}

void ALGWire::autoRearrange(ALGLayout layout) {
    os_log_info(wireLogger, "will auto rearrange: %{public}s",
                description().c_str());
    ALGPoint offset = ALGPoint(offsetX(layout), offsetY(layout));
    ALGPoint origin = leadingNode->position.origin + offset;
    trailingNode->position.finalizeOrigin(origin);
    for (ALGWire* outputWire : trailingNode->outputWiresWithCommonParent()) {
        outputWire->autoRearrange(layout);
    }
    os_log_info(wireLogger, "did auto rearrange: %{public}s",
                description().c_str());
}

// MARK: - Offset

double ALGWire::offsetX(ALGLayout layout) {
    return leadingNode->size(layout).width + layout.spacing;
}

double ALGWire::offsetY(ALGLayout layout) {
    vector<ALGWire*> leadingParallelWires = trailingNode->inputWiresWithCommonParent();
    vector<ALGWire*> trailingParallelWires = leadingNode->outputWiresWithCommonParent();
    double leadingY = 0.0;
    double trailingY = 0.0;
    double leadingHeight = 0.0;
    double trailingHeight = 0.0;
    for (int i = 0; i < leadingParallelWires.size(); i++) {
        ALGWire* leadingParallelWire = leadingParallelWires[i];
        if (leadingParallelWire == this) {
            leadingY = leadingHeight;
        }
        leadingHeight += leadingParallelWire->leadingNode->size(layout).height;
        if (i < leadingParallelWires.size() - 1) {
            leadingHeight += layout.spacing;
        }
    }
    for (int i = 0; i < trailingParallelWires.size(); i++) {
        ALGWire* trailingParallelWire = trailingParallelWires[i];
        if (trailingParallelWire == this) {
            trailingY = trailingHeight;
        }
        trailingHeight += trailingParallelWire->trailingNode->size(layout).height;
        if (i < trailingParallelWires.size() - 1) {
            trailingHeight += layout.spacing;
        }
    }
    double leadingOffsetY = leadingY - leadingHeight / 2 + trailingNode->size(layout).height / 2;
    double trailingOffsetY = trailingY - trailingHeight / 2 + leadingNode->size(layout).height / 2;
    return leadingOffsetY + trailingOffsetY;
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGWire* wire) {
    os << "wire(from: " << wire->leadingNode << " to: " << wire->trailingNode << ")";
    return os;
}

string ALGWire::description() {
    return "wire(from: " + leadingNode->description() + " to: " + trailingNode->description() + ")";
}
