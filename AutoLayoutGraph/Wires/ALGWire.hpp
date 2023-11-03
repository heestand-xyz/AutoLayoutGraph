//
//  ALGWire.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-01.
//

#ifndef ALGWire_hpp
#define ALGWire_hpp
#include "../Nodes/ALGNode.hpp"

using namespace std;

class ALGWire {
    
public:    
    ALGNode* leadingNode;
    ALGNode* trailingNode;
    
    ALGWire(ALGNode* leadingNode, ALGNode* trailingNode);
    
    bool hasCommonParent();
    ALGGroupNode* commonParent();
    ALGNode* leadingNodeWithCommonParent();
    ALGNode* trailingNodeWithCommonParent();
    
    bool isIndirectlyConnected();
    static bool isIndirectlyConnectedWith(ALGWire* wire, ALGNode* checkNode, vector<ALGNode*> checkedNodes);
    
    vector<ALGNode*> leadingNodes();
    vector<ALGNode*> trailingNodes();
    static vector<ALGNode*> nodesWith(ALGWire* wire, ALGNode* checkNode, vector<ALGNode*> checkedNodes);
};

#endif /* ALGWire_hpp */
