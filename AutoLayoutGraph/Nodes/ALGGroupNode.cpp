//
//  ALGGroupNode.cpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#include "ALGGroupNode.hpp"
#include "ALGSize.hpp"

ALGGroupNode::ALGGroupNode(string typeName)
: ALGNode(typeName)
{
}

ALGSize ALGGroupNode::getSize(ALGLayout layout)
{
    // TODO: Implement
    return ALGSize::zero;
}

void ALGGroupNode::autoLayout(ALGLayout layout)
{
    for(ALGNodeSection& section : sections) {
        section.autoLayout(layout);
    }
}
