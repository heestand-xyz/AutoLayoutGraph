//
//  ALGNode.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGNode.hpp"
#include "ALGPoint.hpp"

ALGNode::ALGNode(string typeName)
: origin(ALGPoint::zero)
{
    uuid_generate(id);
    this->typeName = typeName;
}
