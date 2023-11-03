//
//  ALGGroupNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <iostream>
#include "ALGGroupNode.hpp"
#include "ALGNodeSection.hpp"
#include "../Helpers/First.hpp"
#include "../Helpers/Remove.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Equal.hpp"
#include "../Helpers/Append.hpp"

ALGGroupNode::ALGGroupNode(string typeName)
: ALGNode(typeName)
{ }

class ALGGroupNodeException : public runtime_error {
public:
    ALGGroupNodeException(const string& message) : runtime_error(message) {}
};

void ALGGroupNode::add(ALGNode* node) {
    cout << "will add node: " << node->typeName << endl;
    if (node->parent) {
        throw ALGGroupNodeException("Add node failed, node already has a parent.");
    }
    if (!node->inputWires.empty() || !node->outputWires.empty()) {
        throw ALGGroupNodeException("Add node failed, node is connected.");
    }
    ALGNodeSection* section = addSection();
    section->nodes.push_back(node);
    node->parent = this;
    cout << "did add node: " << node->typeName << endl;
}

void ALGGroupNode::remove(ALGNode* node) {
    cout << "will remove node: " << node->typeName << endl;
    if (node->parent == nullptr) {
        throw ALGGroupNodeException("Remove node failed, node parent not found.");
    }
    for (ALGWire* wire : node->inputWires) {
        ALGNode::disconnect(wire);
    }
    for (ALGWire* wire : node->outputWires) {
        ALGNode::disconnect(wire);
    }
    bool didRemove = false;
    for (ALGNodeSection* section : sections) {
        if (containsWhere(section->nodes, [node](ALGNode* sectionNode) {
            return isEqual(sectionNode->id, node->id);
        })) {
            removeIn(section->nodes, node);
            if (section->nodes.empty()) {
                removeSection(section);
            }
            didRemove = true;
            break;
        }
    }
    if (!didRemove) {
        throw ALGGroupNodeException("Remove node failed, not found in group.");
    }
    node->parent = nullptr;
    cout << "did remove node: " << node->typeName << endl;
}

vector<ALGNode*> ALGGroupNode::allNodes() {
    vector<ALGNode*> allNodes = vector<ALGNode*>();
    for (ALGNodeSection* section : sections) {
        allNodes += section->nodes;
    }
    return allNodes;
}

ALGSize ALGGroupNode::size(ALGLayout layout)
{
    if (sections.empty()) {
        return ALGSize::zero;
    }
    ALGSize totalSize = ALGSize::zero;
    for (int i = 0; i < sections.size(); ++i) {
        if (i != 0) {
            totalSize.height += layout.spacing;
        }
        ALGNodeSection* section = sections[i];
        ALGSize sectionSize = section->size(layout);
        totalSize.width = max(totalSize.width, sectionSize.width);
        totalSize.height += sectionSize.height;
    }
    return totalSize.padding(layout.padding);
}

bool ALGGroupNode::contains(ALGNode* node) {
    for (ALGNodeSection* section : sections) {
        if (section->contains(node)) {
            return true;
        }
    }
    return false;
}

bool ALGGroupNode::deepContains(ALGNode* node) {
    for (ALGNodeSection* section : sections) {
        if (section->deepContains(node)) {
            return true;
        }
    }
    return false;
}

bool ALGGroupNode::deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout) {
    ALGPoint sectionOrigin = ALGPoint(layout.padding, layout.padding);
    for (int i = 0; i < sections.size(); i++) {
        ALGNodeSection* section = sections[i];
        if (section->deepHitTest(node, point - sectionOrigin, layout)) {
            return true;
        }
        sectionOrigin.y += section->size(layout).height;
        sectionOrigin.y += layout.spacing;
    }
    return false;
}

ALGRect ALGGroupNode::deepFrame(ALGNode* node, ALGLayout layout) {
    for (int i = 0; i < sections.size(); i++) {
        ALGNodeSection* section = sections[i];
        if (section->deepContains(node)) {
            for (ALGNode* n : section->nodes) {
                if (n == node) {
                    return ALGRect(section->getOrigin(layout), node->size(layout));
                }
                ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(n);
                if (groupNode && groupNode->deepContains(node)) {
                    ALGRect frame = groupNode->deepFrame(node, layout);
                    return ALGRect(section->getOrigin(layout) + frame.origin, node->size(layout));
                }
            }
        }
    }
    throw ALGGroupNodeException("Getting deep frame failed, node not found.");
}

void ALGGroupNode::updateSectionsOnDidConnect(ALGWire* wire) {
    ALGNodeSection* leadingSection = nullptr;
    ALGNodeSection* trailingSection = nullptr;
    for (ALGNodeSection* section : sections) {
        if (section->deepContains(wire->leadingNode) && section->deepContains(wire->trailingNode)) {
            return;
        } else if (section->deepContains(wire->leadingNode)) {
            leadingSection = section;
        } else if (section->deepContains(wire->trailingNode)) {
            trailingSection = section;
        }
    }
    if (leadingSection == nullptr || trailingSection == nullptr) {
        throw ALGGroupNodeException("Update sections on did connect failed, some section not found.");
    }
    leadingSection->nodes += trailingSection->nodes;
    removeSection(trailingSection);
}

void ALGGroupNode::updateSectionsOnDidDisconnect(ALGWire* wire) {
    ALGNodeSection* section = firstWhere(sections, [wire](ALGNodeSection* section) {
        return section->deepContains(wire->leadingNode);
    });
    if (section) {
        if (!wire->isIndirectlyConnected()) {
            ALGNodeSection* newSection = addSection();
            vector<ALGNode*> newNodes = wire->trailingNodes();
            for (ALGNode* newNode : newNodes) {
                removeIn(section->nodes, newNode);
                newSection->nodes.push_back(newNode);
            }
        }
    } else {
        throw ALGGroupNodeException("Update sections on did disconnect failed, section not found.");
    }
}

ALGNodeSection* ALGGroupNode::addSection() {
    ALGNodeSection* section = new ALGNodeSection();
    section->group = this;
    sections.push_back(section);
    return section;
}

void ALGGroupNode::removeSection(ALGNodeSection* section) {
    section->group = nullptr;
    removeIn(sections, section);
    delete section;
}

bool ALGGroupNode::isRoot() {
    return parent == nullptr;
}
