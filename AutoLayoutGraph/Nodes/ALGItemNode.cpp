//
//  ALGItemNode.cpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#include "ALGItemNode.hpp"
#include "ALGSize.hpp"

ALGItemNode::ALGItemNode(string typeName)
: ALGNode(typeName)
{
}


ALGSize ALGItemNode::getSize(ALGLayout layout)
{
    return size.padding(layout.padding);
}
