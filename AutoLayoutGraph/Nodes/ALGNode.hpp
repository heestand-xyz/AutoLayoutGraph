//
//  ALGNode.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#ifndef ALGNode_hpp
#define ALGNode_hpp

#include <uuid/uuid.h>
#include <string>
#include "ALGPoint.hpp"

using namespace std;

class ALGNode {
    
public:
    uuid_t id;
    string typeName;
    ALGPoint origin;
    
    ALGNode(string typeName);
};

#endif /* ALGNode_hpp */
