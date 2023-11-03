//
//  ALGNodeSection.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGNodeSection_hpp
#define ALGNodeSection_hpp

#include <vector>
#include "ALGNode.hpp"
#include "ALGGroupNode.hpp"
#include "../Layout/ALGLayout.hpp"

using namespace std;

struct ALGNodeSection {
    
    ALGGroupNode* group = nullptr;

    vector<ALGNode*> nodes;
    
    bool contains(ALGNode* node);
    bool deepContains(ALGNode* node);
    bool deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout);
    
    ALGPoint getOrigin(ALGLayout layout);
    ALGSize size(ALGLayout layout);
    
    vector<ALGNode*> finalNodes();
    
    void autoLayout(ALGLayout layout);
    
    int index();
};

#endif /* ALGNodeSection_hpp */
