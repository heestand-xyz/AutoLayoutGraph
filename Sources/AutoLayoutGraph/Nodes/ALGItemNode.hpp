//
//  ALGItemNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGItemNode_hpp
#define ALGItemNode_hpp

#include "ALGNode.hpp"
#include "../Layout/Types/ALGSize.hpp"

using namespace std;

class ALGItemNode : public ALGNode {
    
public:
    ALGSize fixedSize;
    
    ALGItemNode(string typeName, ALGSize size);
    
    ALGSize size(ALGLayout layout);
    
    friend ostream& operator<<(ostream& os, const ALGItemNode* itemNode);
};

#endif /* ALGItemNode_hpp */
