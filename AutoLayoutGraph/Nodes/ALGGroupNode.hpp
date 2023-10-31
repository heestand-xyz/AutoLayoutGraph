//
//  ALGGroupNode.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#ifndef ALGGroupNode_hpp
#define ALGGroupNode_hpp

#include <vector>
#include "ALGNode.hpp"
#include "ALGNodeSection.hpp"

using namespace std;

class ALGGroupNode : public ALGNode {
    
public:
    vector<ALGNodeSection> sections;
};

#endif /* ALGGroupNode_hpp */
