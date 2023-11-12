//
//  ALGGroupNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <os/log.h>
#include "ALGGroupNode.hpp"
#include "ALGNodeSection.hpp"
#include "../Helpers/First.hpp"
#include "../Helpers/Remove.hpp"
#include "../Helpers/Contains.hpp"
#include "../Helpers/Append.hpp"

ALGGroupNode::ALGGroupNode(string name, string typeName)
: ALGNode(name, typeName)
{ }

// MARK: - Logger

os_log_t groupNodeLogger = os_log_create("AutoLayoutGraph", "ALGGroupNode");

// MARK: - Exception

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
    ALGNodeSection* s = section();
    return s->origin(layout) + position.originInSection(s);
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
    os_log_fault(groupNodeLogger, "getting deep frame failed, node not found");
    throw ALGGroupNodeException("getting deep frame failed, node not found");
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

void ALGGroupNode::updateSectionsOnConnect(ALGWire* wire) {
    os_log_info(groupNodeLogger, "will update sections on connect for wire: %{public}s",
                wire->description().c_str());
    ALGNodeSection* leadingSection = nullptr;
    ALGNodeSection* trailingSection = nullptr;
    for (ALGNodeSection* section : sections) {
        if (section->deepContains(wire->leadingNode) && section->deepContains(wire->trailingNode)) {
            os_log_info(groupNodeLogger, "no need to update sections on connect for wire: %{public}s",
                        wire->description().c_str());
            return;
        } else if (section->deepContains(wire->leadingNode)) {
            leadingSection = section;
        } else if (section->deepContains(wire->trailingNode)) {
            trailingSection = section;
        }
    }
    if (leadingSection == nullptr || trailingSection == nullptr) {
        os_log_fault(groupNodeLogger, "update sections on did connect failed, some section not found");
        throw ALGGroupNodeException("update sections on did connect failed, some section not found");
    }
    leadingSection->nodes += trailingSection->nodes;
    removeSection(trailingSection);
    os_log_info(groupNodeLogger, "did update sections on connect for wire: %{public}s",
                wire->description().c_str());
}

void ALGGroupNode::updateSectionsOnDisconnect(ALGWire* wire) {
    os_log_info(groupNodeLogger, "will update sections on disconnect for wire: %{public}s",
                wire->description().c_str());
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
        os_log_fault(groupNodeLogger, "update sections on did disconnect failed, section not found");
        throw ALGGroupNodeException("update sections on did disconnect failed, section not found");
    }
    os_log_info(groupNodeLogger, "did update sections on disconnect for wire: %{public}s",
                wire->description().c_str());
}

// MARK: - Section

ALGNodeSection* ALGGroupNode::addSection() {
    ALGNodeSection* section = new ALGNodeSection();
    section->group = this;
    sections.push_back(section);
    os_log_info(groupNodeLogger, "add section: %{public}s to group: %{public}s",
                section->description().c_str(),
                description().c_str());
    return section;
}

void ALGGroupNode::removeSection(ALGNodeSection* section) {
    section->group = nullptr;
    removeIn(sections, section);
    delete section;
}

// MARK: - Is

bool ALGGroupNode::isRoot() {
    if (parent) {
        return false;
    }
    return true;
}

// MARK: - Auto Layout

void ALGGroupNode::autoLayout(ALGLayout layout) {
    os_log_info(groupNodeLogger, "will auto layout: %{public}s",
                description().c_str());
    for (ALGNodeSection* section : sections) {
        cout << "-------->>> " << section << endl;
        section->autoLayout(layout);
    }
    os_log_info(groupNodeLogger, "did auto layout: %{public}s",
                description().c_str());
}

// MARK: - As

ALGNode* ALGGroupNode::asNode() {
    return this;
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGGroupNode* groupNode) {
    os << "groupNode(name: '" << groupNode->name << "', typeName: '" << groupNode->typeName << "')";
    return os;
}

string ALGGroupNode::description() {
    return "groupNode(name: '" + name + "', typeName: '" + typeName + "')";
}
