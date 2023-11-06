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

// MARK: - Auto Layout

void ALGWire::autoLayout(ALGLayout layout) {
    cout << "will auto layout: " << this << endl;
    if (leadingNode->position.state == ALGPositionState::FINAL) {
        autoRearrange(layout);
        return;
    }
    ALGPoint offset = ALGPoint(-offsetX(layout), -offsetY(layout)); //offsetForLeadingNode(layout);
    ALGPoint origin = trailingNode->position.origin + offset;
    if (trailingNode->position.state == ALGPositionState::FINAL) {
        leadingNode->position.finalizeOrigin(origin);
        cout << "auto layout final position of: " << leadingNode << " to: " << origin << endl;
    } else {
        leadingNode->position.temporaryOrigin(origin);
        cout << "auto layout temporary position of: " << leadingNode << " to: " << origin << endl;
    }
    for (ALGWire* inputWire : leadingNode->inputWiresWithCommonParent()) {
        inputWire->autoLayout(layout);
    }
    cout << "did auto layout: " << this << endl;
}

void ALGWire::autoRearrange(ALGLayout layout) {
    cout << "will auto rearrange: " << this << endl;
    ALGPoint offset = ALGPoint(offsetX(layout), offsetY(layout)); //offsetForTrailingNode(layout);
    ALGPoint origin = leadingNode->position.origin + offset;
    trailingNode->position.finalizeOrigin(origin);
    cout << "auto rearrange final position of: " << trailingNode << " to: " << origin << endl;
    for (ALGWire* outputWire : trailingNode->outputWiresWithCommonParent()) {
        outputWire->autoRearrange(layout);
    }
    cout << "did auto rearrange: " << this << endl;
}

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

//ALGPoint ALGWire::offsetForLeadingNode(ALGLayout layout) {
//    double offsetX = -layout.spacing - leadingNode->size(layout).width;
//    vector<ALGWire*> parallelWires = trailingNode->inputWiresWithCommonParent();
//    auto parallelCount = parallelWires.size();
//    double y = 0.0;
//    double height = 0.0;
//    for (int i = 0; i < parallelCount; i++) {
//        ALGWire* parallelWire = parallelWires[i];
//        if (parallelWire == this) {
//            y = height;
//        }
//        height += parallelWire->leadingNode->size(layout).height;
//        if (i < parallelCount - 1) {
//            height += layout.spacing;
//        }
//    }
//    double offsetY = y - height / 2 + trailingNode->size(layout).height / 2;
//    return ALGPoint(offsetX, offsetY);
//}

//ALGPoint ALGWire::offsetForTrailingNode(ALGLayout layout) {
//    double offsetX = leadingNode->size(layout).width + layout.spacing;
//    vector<ALGWire*> parallelWires = leadingNode->outputWiresWithCommonParent();
//    auto parallelCount = parallelWires.size();
//    double y = 0.0;
//    double height = 0.0;
//    for (int i = 0; i < parallelCount; i++) {
//        ALGWire* parallelWire = parallelWires[i];
//        if (parallelWire == this) {
//            y = height;
//        }
//        height += parallelWire->trailingNode->size(layout).height;
//        if (i < parallelCount - 1) {
//            height += layout.spacing;
//        }
//    }
//    double offsetY = y - height / 2 + leadingNode->size(layout).height / 2;
//    return ALGPoint(offsetX, offsetY);
//}

// MARK: - Print

ostream& operator<<(ostream& os, const ALGWire* wire) {
    os << "wire(from: " << wire->leadingNode << " to: " << wire->trailingNode << ")";
    return os;
}
