//
//  ALGItemNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGItemNode_hpp
#define ALGItemNode_hpp

#include "ALGNode.hpp"
#include "../Layout/ALGSize.hpp"

using namespace std;

class ALGItemNode : public ALGNode {
    
public:
    ALGSize size;
    
    ALGItemNode(string typeName, ALGSize size);
    
    ALGSize getSize(ALGLayout layout);
};

#endif /* ALGItemNode_hpp */
