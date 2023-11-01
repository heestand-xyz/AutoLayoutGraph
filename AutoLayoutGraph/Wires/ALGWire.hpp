//
//  ALGWire.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-01.
//

#ifndef ALGWire_hpp
#define ALGWire_hpp
#include "../Nodes/ALGNode.hpp"
#include "ALGWireLength.hpp"

using namespace std;

class ALGWire {
    
public:
    ALGWireLength length;
    
    ALGNode* leadingNode;
    ALGNode* trailingNode;
    
    ALGWire(ALGNode* leadingNode, ALGNode* trailingNode);
};

#endif /* ALGWire_hpp */
