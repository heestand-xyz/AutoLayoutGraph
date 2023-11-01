//
//  ALGNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGNode_hpp
#define ALGNode_hpp

#include <uuid/uuid.h>
#include <string>
#include "../Layout/ALGPoint.hpp"
#include "../Layout/ALGSize.hpp"
#include "../Layout/ALGLayout.hpp"

using namespace std;

class ALGNode {
    
public:
    uuid_t id;
    string typeName;
    ALGPoint origin;
    
    ALGNode(string typeName);
    virtual ~ALGNode() {}
    
    virtual ALGSize getSize(ALGLayout layout) = 0;
};

#endif /* ALGNode_hpp */
