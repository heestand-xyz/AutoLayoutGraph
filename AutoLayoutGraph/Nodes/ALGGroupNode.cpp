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

void ALGGroupNode::add(ALGNode* node) {
    cout << "will add node: " << node->typeName << endl;
    ALGNodeSection section = ALGNodeSection();
    section.nodes.push_back(node);
    sections.push_back(section);
    cout << "did add node: " << node->typeName << endl;
}

ALGSize ALGGroupNode::getSize(ALGLayout layout)
{
    if (sections.empty()) {
        return ALGSize::zero;
    }
    ALGSize totalSize = ALGSize::zero;
    for (int i = 0; i < sections.size(); ++i) {
        if (i != 0) {
            totalSize.height += layout.spacing;
        }
        ALGNodeSection section = sections[i];
        ALGSize sectionSize = section.getSize(layout);
        totalSize.width = max(totalSize.width, sectionSize.width);
        totalSize.height += sectionSize.height;
    }
    return totalSize.padding(layout.padding);
}

void ALGGroupNode::autoLayout(ALGLayout layout)
{
    for (ALGNodeSection& section : sections) {
        section.autoLayout(layout);
    }
}
