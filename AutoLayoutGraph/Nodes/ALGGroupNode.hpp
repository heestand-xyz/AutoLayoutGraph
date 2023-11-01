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
#include "ALGNodeSection.hpp"
#include "../Layout/ALGLayout.hpp"

using namespace std;

class ALGGroupNode : public ALGNode {
    
public:
    vector<ALGNodeSection> sections;
    
    ALGGroupNode(string typeName);
    
    void add(ALGNode* node);
    void remove(ALGNode* node);
    
    void autoLayout(ALGLayout layout);
    
    ALGSize getSize(ALGLayout layout);
};

#endif /* ALGGroupNode_hpp */
