//
//  ALGItemNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGItemNode_hpp
#define ALGItemNode_hpp

#include <swift/bridging>
#include "ALGNode.hpp"
#include "../Layout/Types/ALGSize.hpp"

using namespace std;

class ALGItemNode : public ALGNode {
    
public:
    
    ALGSize fixedSize;
    
    ALGItemNode(string typeName, ALGSize size);
    
    ~ALGItemNode() {
//        cout << "deinit of item " << this << endl;
    }
    
    void update(ALGSize size, ALGLayout layout);
    ALGSize size(ALGLayout layout);
    ALGPoint origin(ALGLayout layout);
    
    ALGNode* asNode() SWIFT_RETURNS_INDEPENDENT_VALUE;
    
    bool hasParent() SWIFT_RETURNS_INDEPENDENT_VALUE;
    
    friend ostream& operator<<(ostream& os, const ALGItemNode* itemNode);
};

#endif /* ALGItemNode_hpp */
