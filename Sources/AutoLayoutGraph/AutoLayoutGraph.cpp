//
//  AutoLayoutGraph.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#include <os/log.h>
#include "AutoLayoutGraph.hpp"
#include "Helpers/Remove.hpp"
#include "Nodes/ALGNodeSection.hpp"
#include "Helpers/Contains.hpp"

AutoLayoutGraph::AutoLayoutGraph(ALGLayout layout)
: layout(layout)
{}

// MARK: - Logger

os_log_t logger = os_log_create("AutoLayoutGraph", "AutoLayoutGraph");

// MARK: - Exception

class AutoLayoutGraphException : public runtime_error {
public:
    AutoLayoutGraphException(const string& message) : runtime_error(message) {}
};

// MARK: - Life Cycle

void AutoLayoutGraph::addToParent(ALGGroupNode* parentNode, ALGNode* node) {
    os_log_info(logger, "will add: %{public}s to parent: %{public}s",
                node->description().c_str(),
                parentNode->description().c_str());
    if (node->parent) {
        os_log_fault(logger, "add node failed, node already has a parent: %{public}s",
                     node->parent->description().c_str());
        throw AutoLayoutGraphException("add node failed, node already has a parent");
    }
    if (!node->inputWires.empty() || !node->outputWires.empty()) {
        os_log_fault(logger, "add node failed, node is connected");
        throw AutoLayoutGraphException("add node failed, node is connected");
    }
    ALGNodeSection* section = parentNode->addSection();
    section->nodes.push_back(node);
    node->parent = parentNode;
    node->root()->autoLayout(layout);
    os_log_info(logger, "did add: %{public}s to parent: %{public}s",
                node->description().c_str(),
                parentNode->description().c_str());
}

void AutoLayoutGraph::moveToNewParent(ALGGroupNode* parentNode, ALGNode* node) {
    // TODO: Implement
}

void AutoLayoutGraph::removeFromParent(ALGNode* node) {
    os_log_info(logger, "will remove: %{public}s from parent: %{public}s",
                node->description().c_str(),
                node->parent->description().c_str());
    if (node->parent == nullptr) {
        os_log_fault(logger, "remove node failed, node parent not found");
        throw AutoLayoutGraphException("remove node failed, node parent not found");
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
        os_log_fault(logger, "remove node failed, not found in group");
        throw AutoLayoutGraphException("remove node failed, not found in group");
    }
    node->root()->autoLayout(layout);
    os_log_info(logger, "did remove: %{public}s from parent: %{public}s",
                node->description().c_str(),
                node->parent->description().c_str());
    node->parent = nullptr;
}

// MARK: - Connection

void AutoLayoutGraph::connectWire(ALGNode* leadingNode, ALGNode* trailingNode) {
    os_log_info(logger, "will connect wire from leading: %{public}s to trailing: %{public}s",
                leadingNode->description().c_str(),
                trailingNode->description().c_str());
    if (leadingNode->root() != trailingNode->root()) {
        os_log_fault(logger, "connection failed, no common root group node");
        throw AutoLayoutGraphException("connection failed, no common root group node");
    }
    if (AutoLayoutGraph::isConnected(leadingNode, trailingNode)) {
        os_log_fault(logger, "connection failed, already connected");
        throw AutoLayoutGraphException("connection failed, already connected");
    }
    if (AutoLayoutGraph::isLoop(leadingNode, trailingNode)) {
        os_log_fault(logger, "connection failed, loop found");
        throw AutoLayoutGraphException("connection failed, loop found");
    }
    ALGWire* wire = new ALGWire(leadingNode, trailingNode);
    wire->leadingNode->outputWires.push_back(wire);
    wire->trailingNode->inputWires.push_back(wire);
    ALGGroupNode* commonParent = wire->commonParent();
    commonParent->updateSectionsOnConnect(wire);
    commonParent->root()->autoLayout(layout);
    os_log_info(logger, "did connect wire from leading: %{public}s to trailing: %{public}s",
                leadingNode->description().c_str(),
                trailingNode->description().c_str());
}

void AutoLayoutGraph::disconnectWire(ALGNode* leadingNode, ALGNode* trailingNode) {
    os_log_info(logger, "will disconnect wire from leading: %{public}s to trailing: %{public}s",
                leadingNode->description().c_str(),
                trailingNode->description().c_str());
    if (!AutoLayoutGraph::isConnected(leadingNode, trailingNode)) {
        os_log_fault(logger, "disconnect failed, already disconnected");
        throw AutoLayoutGraphException("disconnect failed, already disconnected");
    }
    ALGWire* wire = optionalWire(leadingNode, trailingNode);
    if (wire == nullptr) {
        os_log_fault(logger, "disconnect failed, wire not found");
        throw AutoLayoutGraphException("disconnect failed, wire not found");
    }
    disconnectWire(wire);
    delete wire;
    os_log_info(logger, "did disconnect wire from leading: %{public}s to trailing: %{public}s",
                leadingNode->description().c_str(),
                trailingNode->description().c_str());
}

void AutoLayoutGraph::disconnectWire(ALGWire* wire) {
    removeIn(wire->leadingNode->outputWires, wire);
    removeIn(wire->trailingNode->inputWires, wire);
    ALGGroupNode* commonParent = wire->commonParent();
    commonParent->updateSectionsOnDisconnect(wire);
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
