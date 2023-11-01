//
//  ALGWire.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-11-01.
//

#ifndef ALGWire_hpp
#define ALGWire_hpp

#include "../Nodes/ALGNode.hpp"
#include "ALGWireLength.hpp"

using namespace std;

class ALGWire {
    
public:
    ALGWireLength length;
    ALGNode* upstreamNode;
    ALGNode* downstreamNode;
};

#endif /* ALGWire_hpp */
