//
//  ALGNodeSection.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#ifndef ALGNodeSection_hpp
#define ALGNodeSection_hpp

#include <vector>
#include "ALGNode.hpp"
#include "ALGLayout.hpp"

using namespace std;

struct ALGNodeSection {
    
    vector<ALGNode> nodes;
    
    ALGSize getSize();
    void autoLayout(ALGLayout layout);
};

#endif /* ALGNodeSection_hpp */
