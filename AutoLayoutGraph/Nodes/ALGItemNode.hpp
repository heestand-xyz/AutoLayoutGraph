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
    ALGSize size = ALGSize::zero;
    
    ALGItemNode(string typeName);
    
private:
    ALGSize getSize(ALGLayout layout);
};

#endif /* ALGItemNode_hpp */
