//
//  AutoLayoutGraph.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#include "AutoLayoutGraph.hpp"
#include "Helpers/Remove.hpp"
#include "Nodes/ALGNodeSection.hpp"
#include "Helpers/Contains.hpp"

AutoLayoutGraph::AutoLayoutGraph(ALGLayout layout)
: layout(layout)
{}

// MARK: - Exception

class AutoLayoutGraphException : public runtime_error {
public:
    AutoLayoutGraphException(const string& message) : runtime_error(message) {}
};

// MARK: - Life Cycle

void AutoLayoutGraph::addToParent(ALGGroupNode* parentNode, ALGNode* node) {
    cout << "will add node: " << node->typeName << " to parent: " << parentNode->typeName << endl;
    if (node->parent) {
        throw AutoLayoutGraphException("Add node failed, node already has a parent.");
    }
    if (!node->inputWires.empty() || !node->outputWires.empty()) {
        throw AutoLayoutGraphException("Add node failed, node is connected.");
    }
    ALGNodeSection* section = parentNode->addSection();
    section->nodes.push_back(node);
    node->parent = parentNode;
    node->root()->autoLayout(layout);
    cout << "did add node: " << node->typeName << " to parent: " << parentNode->typeName << endl;
}

void AutoLayoutGraph::moveToNewParent(ALGGroupNode* parentNode, ALGNode* node) {
    // TODO: Implement
}

void AutoLayoutGraph::removeFromParent(ALGNode* node) {
    cout << "will remove node: " << node->typeName << " from parent: " << node->parent->typeName << endl;
    if (node->parent == nullptr) {
        throw AutoLayoutGraphException("Remove node failed, node parent not found.");
    }
    for (ALGWire* wire : node->inputWires) {
        disconnectWire(wire);
        delete wire;
    }
    for (ALGWire* wire : node->outputWires) {
        disconnectWire(wire);
        delete wire;
    }
    bool didRemove = false;
    for (ALGNodeSection* section : node->parent->sections) {
        if (containsWhere(section->nodes, [node](ALGNode* sectionNode) {
            return sectionNode == node;
        })) {
            removeIn(section->nodes, node);
            if (section->nodes.empty()) {
                node->parent->removeSection(section);
            }
            didRemove = true;
            break;
        }
    }
    if (!didRemove) {
        throw AutoLayoutGraphException("Remove node failed, not found in group.");
    }
    node->root()->autoLayout(layout);
    cout << "did remove node: " << node->typeName << " from parent: " << node->parent->typeName << endl;
    node->parent = nullptr;
}

// MARK: - Connection

void AutoLayoutGraph::connectWire(ALGNode* leadingNode, ALGNode* trailingNode) {
    cout << "will connect wire from leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
    if (leadingNode->root() != trailingNode->root()) {
        throw AutoLayoutGraphException("Connection failed, no common root group node.");
    }
    if (AutoLayoutGraph::isConnected(leadingNode, trailingNode)) {
        throw AutoLayoutGraphException("Connection failed, already connected.");
    }
    if (AutoLayoutGraph::isLoop(leadingNode, trailingNode)) {
        throw AutoLayoutGraphException("Connection failed, loop found.");
    }
    ALGWire* wire = new ALGWire(leadingNode, trailingNode);
    wire->leadingNode->outputWires.push_back(wire);
    wire->trailingNode->inputWires.push_back(wire);
    ALGGroupNode* commonParent = wire->commonParent();
    commonParent->updateSectionsOnDidConnect(wire);
    commonParent->root()->autoLayout(layout);
    cout << "did connect wire from leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

void AutoLayoutGraph::disconnectWire(ALGNode* leadingNode, ALGNode* trailingNode) {
    cout << "will disconnect wire from leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
    if (!AutoLayoutGraph::isConnected(leadingNode, trailingNode)) {
        throw AutoLayoutGraphException("Disconnect failed, already disconnected.");
    }
    ALGWire* wire = optionalWire(leadingNode, trailingNode);
    if (wire == nullptr) {
        throw AutoLayoutGraphException("Disconnect failed, wire not found.");
    }
    disconnectWire(wire);
    delete wire;
    cout << "did disconnect wire from leading node: " << leadingNode->typeName << " to trailing node: " << trailingNode->typeName << endl;
}

void AutoLayoutGraph::disconnectWire(ALGWire* wire) {
    removeIn(wire->leadingNode->outputWires, wire);
    removeIn(wire->trailingNode->inputWires, wire);
    ALGGroupNode* commonParent = wire->commonParent();
    commonParent->updateSectionsOnDidDisconnect(wire);
    commonParent->root()->autoLayout(layout);
}

// MARK: - Wire

ALGWire* AutoLayoutGraph::optionalWire(ALGNode* leadingNode, ALGNode* trailingNode) {
    for (ALGWire* wire : leadingNode->outputWires) {
        if (wire->trailingNode == trailingNode) {
            return wire;
        }
    }
    return nullptr;
}

// MARK: - Is

bool AutoLayoutGraph::isConnected(ALGNode* leadingNode, ALGNode* trailingNode) {
    return optionalWire(leadingNode, trailingNode) != nullptr;
}

bool AutoLayoutGraph::isLoop(ALGNode* leadingNode, ALGNode* trailingNode) {
    if (trailingNode->containsDownstream(leadingNode)) {
        return true;
    }
    if (leadingNode->containsUpstream(trailingNode)) {
        return true;
    }
    return false;
}
