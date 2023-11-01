//
//  ALGGroupNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <iostream>
#include "ALGGroupNode.hpp"
#include "ALGSize.hpp"
#include "../Helpers/Remove.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Equal.hpp"

ALGGroupNode::ALGGroupNode(string typeName)
: ALGNode(typeName)
{ }

class GroupException : public std::runtime_error {
public:
    GroupException(const std::string& message) : std::runtime_error(message) {}
};

void ALGGroupNode::add(ALGNode* node) {
    cout << "will add node: " << node->typeName << endl;
    if (!node->inputWires.empty() || !node->outputWires.empty()) {
        throw GroupException("Add node failed, node is connected.");
    }
    ALGNodeSection section = ALGNodeSection();
    section.nodes.push_back(node);
    sections.push_back(section);
    cout << "did add node: " << node->typeName << endl;
}

void ALGGroupNode::remove(ALGNode* node) {
    cout << "will remove node: " << node->typeName << endl;
    for (ALGWire* wire : node->inputWires) {
        ALGNode::disconnect(wire);
    }
    for (ALGWire* wire : node->outputWires) {
        ALGNode::disconnect(wire);
    }
    bool didRemove = false;
    for (ALGNodeSection section : sections) {
        if (containsWhere(section.nodes, [node](ALGNode* sectionNode) {
            return isEqual(sectionNode->id, node->id);
        })) {
            removeIn(section.nodes, node);
            didRemove = true;
            break;
        }
    }
    if (!didRemove) {
        throw GroupException("Remove node failed, not found.");
    }
    cout << "did remove node: " << node->typeName << endl;
}

ALGSize ALGGroupNode::getSize(ALGLayout layout)
{
    if (sections.empty()) {
        return ALGSize::zero;
    }
    ALGSize totalSize = ALGSize::zero;
    for (int i = 0; i < sections.size(); ++i) {
        if (i != 0) {
            totalSize.height += layout.spacing;
        }
        ALGNodeSection section = sections[i];
        ALGSize sectionSize = section.getSize(layout);
        totalSize.width = max(totalSize.width, sectionSize.width);
        totalSize.height += sectionSize.height;
    }
    return totalSize.padding(layout.padding);
}

void ALGGroupNode::autoLayout(ALGLayout layout)
{
    for (ALGNodeSection& section : sections) {
        section.autoLayout(layout);
    }
}
