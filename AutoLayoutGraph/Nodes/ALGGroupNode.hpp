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
#include "../Layout/ALGLayout.hpp"
#include "../Layout/ALGSize.hpp"

class ALGNodeSection;

using namespace std;

class ALGGroupNode : public ALGNode {
    
public:
    vector<ALGNodeSection> sections;
    
    ALGGroupNode(string typeName);
    
    void add(ALGNode* node);
    void remove(ALGNode* node);
    
    bool contains(ALGNode* node);
    bool deepContains(ALGNode* node);
    bool deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout);
        
    ALGSize getSize(ALGLayout layout);
};

#endif /* ALGGroupNode_hpp */
