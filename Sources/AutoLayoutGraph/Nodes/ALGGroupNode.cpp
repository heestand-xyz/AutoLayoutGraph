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
#include "../Helpers/Append.hpp"

ALGGroupNode::ALGGroupNode(string typeName)
: ALGNode(typeName)
{ }

class ALGGroupNodeException : public runtime_error {
public:
    ALGGroupNodeException(const string& message) : runtime_error(message) {}
};

// MARK: - Nodes

vector<ALGNode*> ALGGroupNode::allNodes() {
    vector<ALGNode*> allNodes = vector<ALGNode*>();
    for (ALGNodeSection* section : sections) {
        allNodes += section->nodes;
    }
    return allNodes;
}

// MARK: - Layout

ALGPoint ALGGroupNode::origin(ALGLayout layout) {
    return position.originInSection(section());
}

ALGSize ALGGroupNode::size(ALGLayout layout) {
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

ALGSize ALGGroupNode::groupSize(ALGLayout layout) {
    return size(layout);
}

ALGRect ALGGroupNode::deepFrame(ALGNode* node, ALGLayout layout) {
    for (ALGNodeSection* section : sections) {
        if (section->deepContains(node)) {
            for (ALGNode* n : section->nodes) {
                if (n == node) {
                    return ALGRect(section->origin(layout) + node->position.originInSection(section), node->size(layout));
                }
                ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(n);
                if (groupNode && groupNode->deepContains(node)) {
                    ALGRect frame = groupNode->deepFrame(node, layout);
                    return ALGRect(section->origin(layout) + frame.origin, frame.size);
                }
            }
        }
    }
    cout << "Failure - Getting deep frame failed, node not found." << endl;
    throw ALGGroupNodeException("Getting deep frame failed, node not found.");
}

// MARK: - Contains

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

// MARK: - Hit Test

bool ALGGroupNode::deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout) {
    for (ALGNodeSection* section : sections) {
        if (section->deepHitTest(node, point - section->origin(layout), layout)) {
            return true;
        }
    }
    return false;
}

// MARK: - Nodes

vector<ALGNode*> ALGGroupNode::deepNodes() {
    vector<ALGNode*> deepNodes = vector<ALGNode*>();
    for (ALGNodeSection* section : sections) {
        deepNodes += section->deepNodes();
    }
    return deepNodes;
}

// MARK: - Update Sections

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
        cout << "Failure - Update sections on did connect failed, some section not found." << endl;
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
            vector<ALGNode*> newNodes = wire->trailingNodesWithCommonParent();
            for (ALGNode* newNode : newNodes) {
                removeIn(section->nodes, newNode);
                newSection->nodes.push_back(newNode);
            }
        }
    } else {
        cout << "Failure - Update sections on did disconnect failed, section not found." << endl;
        throw ALGGroupNodeException("Update sections on did disconnect failed, section not found.");
    }
}

// MARK: - Section

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

// MARK: - Is

bool ALGGroupNode::isRoot() {
    return parent == nullptr;
}

// MARK: - Auto Layout

void ALGGroupNode::autoLayout(ALGLayout layout) {
    cout << "will auto layout: " << this << endl;
    for (ALGNodeSection* section : sections) {
        section->autoLayout(layout);
    }
    cout << "did auto layout: " << this << endl;
}

// MARK: - As

ALGNode* ALGGroupNode::asNode() {
    return this;
}

// MARK: - Print

ostream& operator<<(ostream& os, const ALGGroupNode* groupNode) {
    os << "groupNode('" << groupNode->typeName << "')";
    return os;
}
