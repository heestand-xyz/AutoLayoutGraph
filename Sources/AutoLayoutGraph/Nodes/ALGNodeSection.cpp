//
//  ALGNodeSection.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <os/log.h>
#include "ALGNodeSection.hpp"
#include "ALGGroupNode.hpp"
#include "../Layout/Types/ALGRect.hpp"
#include "../Helpers/Append.hpp"

// MARK: - Logger

os_log_t nodeSectionLogger = os_log_create("AutoLayoutGraph", "ALGNodeSection");

// MARK: - Exception

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
        if (node->outputWiresWithCommonParent().empty()) {
            finalNodes.push_back(node);
        }
    }
    return finalNodes;
}

vector<ALGNode*> ALGNodeSection::deepNodes() {
    vector<ALGNode*> deepNodes = vector<ALGNode*>();
    for (ALGNode* node : nodes) {
        deepNodes.push_back(node);
        ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(node);
        if (groupNode) {
            deepNodes += groupNode->deepNodes();
        }
    }
    return deepNodes;
}

// MARK: - Auto Layout

void ALGNodeSection::autoLayout(ALGLayout layout) {
    os_log_info(nodeSectionLogger, "will auto layout: %{public}s in group: %{public}s",
                description().c_str(),
                group->description().c_str());
    for (ALGNode* node : nodes) {
        ALGGroupNode* groupNode = dynamic_cast<ALGGroupNode*>(node);
        if (groupNode) {
            groupNode->autoLayout(layout);
        }
    }
    for (ALGNode* node : nodes) {
        node->position.reset();
    }
    bool isFirst = true;
    for (ALGNode* finalNode : finalNodes()) {
        if (isFirst) {
            finalNode->position.finalizeOrigin(ALGPoint::zero);
            isFirst = false;
        } else {
            finalNode->position.temporaryOrigin(ALGPoint::zero);
        }
        for (ALGWire* inputWire : finalNode->inputWiresWithCommonParent()) {
            inputWire->autoLayout(layout);
        }
    }
    os_log_info(nodeSectionLogger, "did auto layout: %{public}s in group: %{public}s",
                description().c_str(),
                group->description().c_str());
}

// MARK: - Index

int ALGNodeSection::index() const {
    for (int i = 0; i < group->sections.size(); i++) {
        ALGNodeSection* section = group->sections[i];
        if (section == this) {
            return i;
        }
    }
    os_log_fault(nodeSectionLogger, "section index not found");
    throw ALGNodeSectionException("section index not found");
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGNodeSection* section) {
    os << "section(index: " << section->index() << ")";
    return os;
}

string ALGNodeSection::description() {
    return "section(index: " + to_string(index()) + ")";
}
