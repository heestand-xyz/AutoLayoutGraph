//
//  ALGNodeSection.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGNodeSection.hpp"
#include "ALGGroupNode.hpp"
#include "ALGRect.hpp"

class ALGNodeSectionException : public runtime_error {
public:
    ALGNodeSectionException(const string& message) : runtime_error(message) {}
};

// MARK: - Contains

bool ALGNodeSection::contains(ALGNode* node) {
    for (ALGNode* n : nodes) {
        if (n == node) {
            return true;
        }
    }
    return false;
}

bool ALGNodeSection::deepContains(ALGNode* node) {
    if (contains(node)) {
        return true;
    }
    for (ALGNode* n : nodes) {
        ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(n);
        if (groupNode) {
            if (groupNode->deepContains(node)) {
                return true;
            }
        }
    }
    return false;
}

// MARK: - Hit Test

bool ALGNodeSection::deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout) {
    for (ALGNode* n : nodes) {
        if (n == node) {
            return node->hitTest(point, layout);
        } else {
            ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(n);
            if (groupNode) {
                if (groupNode->deepHitTest(node, point, layout)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// MARK: - Layout

ALGPoint ALGNodeSection::origin(ALGLayout layout) {
    double y = layout.padding;
    for (ALGNodeSection* section : group->sections) {
        if (section == this) {
            break;
        }
        y += section->size(layout).height;
        y += layout.spacing;
    }
    double x = group->size(layout).width / 2 - size(layout).width / 2;
    return ALGPoint(x, y);
}

ALGSize ALGNodeSection::size(ALGLayout layout) {

    if (nodes.empty()) {
        return ALGSize::zero;
    }

    ALGRect* totalFrame = nullptr;
    for (ALGNode* node : nodes) {
        ALGPoint origin = node->position.originInSection(this);
        ALGSize size = node->size(layout);
        if (totalFrame == nullptr) {
            totalFrame = new ALGRect(origin, size);
        } else {
            totalFrame->origin.x = min(totalFrame->origin.x, origin.x);
            totalFrame->origin.y = min(totalFrame->origin.y, origin.y);
            totalFrame->size.width = max(totalFrame->size.width, origin.x + size.width - totalFrame->origin.x);
            totalFrame->size.height = max(totalFrame->size.height, origin.y + size.height - totalFrame->origin.y);
        }
    }
    
    ALGSize totalSize = totalFrame->size;
    delete totalFrame;
    
    return totalSize;
}

ALGRect ALGNodeSection::frame(ALGLayout layout) {
    return ALGRect(origin(layout), size(layout));
}

// MARK: - Nodes

vector<ALGNode*> ALGNodeSection::finalNodes() {
    vector<ALGNode*> finalNodes = vector<ALGNode*>();
    for (ALGNode* node : nodes) {
        if (node->outputWires.empty()) {
            finalNodes.push_back(node);
        }
    }
    return finalNodes;
}

// MARK: - Auto Layout

void ALGNodeSection::autoLayout(ALGLayout layout) {
    cout << "will auto layout: " << this << endl;
    for (ALGNode* node : nodes) {
        ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(node);
        if (groupNode) {
            groupNode->autoLayout(layout);
        }
    }
    for (ALGNode* node : nodes) {
        node->position.state = ALGPositionState::NONE;
    }
    bool isFirst = true;
    for (ALGNode* finalNode : finalNodes()) {
        finalNode->position.origin = ALGPoint::zero;
        if (isFirst) {
            finalNode->position.state = ALGPositionState::FINAL;
            isFirst = false;
        } else {
            finalNode->position.state = ALGPositionState::AUTO;
        }
        for (ALGWire* inputWire : finalNode->inputWires) {
            inputWire->autoLayout(layout);
        }
    }
    cout << "did auto layout: " << this << endl;
}

// MARK: - Index

int ALGNodeSection::index() const {
    
    for (int i = 0; i < group->sections.size(); i++) {
        ALGNodeSection* section = group->sections[i];
        if (section == this) {
            return i;
        }
    }
    
    throw ALGNodeSectionException("Section index not found.");
}

// MARK: - Print

ostream& operator<<(ostream& os, const ALGNodeSection* section) {
    os << "section(" << section->index() << ")";
    return os;
}
