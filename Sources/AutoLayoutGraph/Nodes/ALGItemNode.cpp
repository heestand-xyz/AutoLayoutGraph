//
//  ALGItemNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGItemNode.hpp"
#include "../Layout/Types/ALGSize.hpp"
#include "ALGGroupNode.hpp"

ALGItemNode::ALGItemNode(string typeName, ALGSize size)
: ALGNode(typeName), fixedSize(size)
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
    return position.originInSection(section());
}

// MARK: - As

ALGNode* ALGItemNode::asNode() {
    return this;
}

// MARK: - Print

ostream& operator<<(ostream& os, const ALGItemNode* itemNode) {
    os << "itemNode('" << itemNode->typeName << "')";
    return os;
}

