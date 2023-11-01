//
//  ALGGroupNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <iostream>
#include "ALGGroupNode.hpp"
#include "ALGSize.hpp"

ALGGroupNode::ALGGroupNode(string typeName)
: ALGNode(typeName)
{
}

void ALGGroupNode::add(ALGNode node) {
    std::cout << "will add node: " << node.typeName << endl;
    ALGNodeSection section = ALGNodeSection();
    section.nodes.push_back(&node);
    sections.push_back(section);
    std::cout << "did add node: " << node.typeName << endl;
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
