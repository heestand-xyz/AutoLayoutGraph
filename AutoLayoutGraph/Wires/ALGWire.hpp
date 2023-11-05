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
    
    vector<ALGNode*> leadingNodesWithCommonParent();
    vector<ALGNode*> trailingNodesWithCommonParent();
    static vector<ALGNode*> nodesWithCommonParent(ALGWire* wire, ALGNode* checkNode, vector<ALGNode*> checkedNodes);
    
    void autoLayout(ALGLayout layout);
    void autoRearrange(ALGLayout layout);
    ALGPoint autoOffset(ALGLayout layout);
    
    friend ostream& operator<<(ostream& os, const ALGWire* wire);
};

#endif /* ALGWire_hpp */
