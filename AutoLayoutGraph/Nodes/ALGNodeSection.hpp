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
#include "../Layout/ALGLayout.hpp"

using namespace std;

struct ALGNodeSection {
    
    vector<ALGNode*> nodes;
        
    ~ALGNodeSection() {
        for (ALGNode* node : nodes) {
            delete node;
        }
    }
    
    ALGSize getSize(ALGLayout layout);
    void autoLayout(ALGLayout layout);
};

#endif /* ALGNodeSection_hpp */
