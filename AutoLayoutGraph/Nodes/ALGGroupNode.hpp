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
#include "ALGLayout.hpp"

using namespace std;

class ALGGroupNode : public ALGNode {
    
public:
    vector<ALGNodeSection> sections;
    
    ALGGroupNode(string typeName);
    
    void autoLayout(ALGLayout layout);
    
private:
    ALGSize getSize();
};

#endif /* ALGGroupNode_hpp */
