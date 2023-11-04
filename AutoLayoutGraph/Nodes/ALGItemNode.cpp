//
//  ALGItemNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGItemNode.hpp"
#include "ALGSize.hpp"

ALGItemNode::ALGItemNode(string typeName, ALGSize size)
: ALGNode(typeName), fixedSize(size)
{
}


ALGSize ALGItemNode::size(ALGLayout layout)
{
    return fixedSize;
}

// MARK: - Print

ostream& operator<<(ostream& os, const ALGItemNode* itemNode) {
    os << "itemNode('" << itemNode->typeName << "')";
    return os;
}

