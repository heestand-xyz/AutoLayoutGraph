//
//  ALGGroupNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGGroupNode_hpp
#define ALGGroupNode_hpp

#include <vector>
#include "ALGNode.hpp"
#include "../Wires/ALGWire.hpp"
#include "../Layout/ALGLayout.hpp"
#include "../Layout/Types/ALGSize.hpp"

class ALGNodeSection;

using namespace std;

class ALGGroupNode : public ALGNode {
    
public:
    vector<ALGNodeSection*> sections;
    
    ALGGroupNode(string typeName);
    
    void add(ALGNode* node);
    void remove(ALGNode* node);
    
    vector<ALGNode*> allNodes();
    
    bool contains(ALGNode* node);
    bool deepContains(ALGNode* node);
    bool deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout);
        
    ALGSize size(ALGLayout layout);
    ALGRect deepFrame(ALGNode* node, ALGLayout layout);
    
    void updateSectionsOnDidConnect(ALGWire* wire);
    void updateSectionsOnDidDisconnect(ALGWire* wire);
    
    ALGNodeSection* addSection();
    void removeSection(ALGNodeSection* section);
    
    bool isRoot();
    
    void autoLayout(ALGLayout layout);
};

#endif /* ALGGroupNode_hpp */
