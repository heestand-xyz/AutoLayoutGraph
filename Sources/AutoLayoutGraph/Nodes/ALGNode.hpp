//
//  ALGNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGNode_hpp
#define ALGNode_hpp

#include <swift/bridging>
#include "../Layout/ALGLayout.hpp"
#include "../Layout/Types/ALGPoint.hpp"
#include "../Layout/Types/ALGSize.hpp"
#include "../Layout/Types/ALGRect.hpp"
#include "../Layout/Position/ALGPosition.hpp"

using namespace std;

class ALGGroupNode;
class ALGWire;

class ALGNode {
    
public:
    string name;
    string typeName;
    ALGPosition position;
    
    ALGGroupNode* parent = nullptr;
    
    vector<ALGWire*> inputWires;
    vector<ALGWire*> outputWires;
    
    ALGNode(string name, string typeName);
    
    virtual ~ALGNode() {
        cout << "deinit of " << this << endl;
    }
    
    virtual ALGSize size(ALGLayout layout) = 0;
    bool hitTest(ALGPoint point, ALGLayout layout);
    
    bool containsDownstream(ALGNode* node);
    bool containsUpstream(ALGNode* node);
    
    vector<ALGWire*> inputWiresWithCommonParent();
    vector<ALGWire*> outputWiresWithCommonParent();
    
    ALGGroupNode* root();
    
    ALGNodeSection* section() SWIFT_RETURNS_INDEPENDENT_VALUE;
    
    friend ostream& operator<<(ostream& os, const ALGNode* node);
    string description();
};

#endif /* ALGNode_hpp */
