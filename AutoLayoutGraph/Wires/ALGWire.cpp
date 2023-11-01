//
//  ALGWire.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-01.
//

#include "ALGWire.hpp"

ALGWire::ALGWire(ALGNode* leadingNode, ALGNode* trailingNode) :
length(ALGWireLength::AUTO),
leadingNode(leadingNode),
trailingNode(trailingNode)
{ }
