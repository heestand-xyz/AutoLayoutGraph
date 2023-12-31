//
//  ALGItemNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGItemNode.hpp"
#include "../Layout/Types/ALGSize.hpp"
#include "ALGGroupNode.hpp"
#include "ALGNodeSection.hpp"

ALGItemNode::ALGItemNode(string name, string typeName, ALGSize size)
: ALGNode(name, typeName), fixedSize(size)
{}

// MARK: - Layout

void ALGItemNode::update(ALGSize size, ALGLayout layout) {
    fixedSize = size;
    parent->root()->autoLayout(layout);
}

ALGSize ALGItemNode::size(ALGLayout layout) {
    return fixedSize;
}

ALGPoint ALGItemNode::origin(ALGLayout layout) {
    ALGNodeSection* s = section();
    return s->origin(layout) + position.originInSection(s);
}

// MARK: - As

ALGNode* ALGItemNode::asNode() {
    return this;
}

// MARK: - Has

bool ALGItemNode::hasParent() {
    if (parent) {
        return true;
    }
    return false;
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGItemNode* itemNode) {
    os << "itemNode(name: '" << itemNode->name << "', typeName: '" << itemNode->typeName << "')";
    return os;
}

string ALGItemNode::description() {
    return "itemNode(name: '" + name + "', typeName: '" + typeName + "')";
}
