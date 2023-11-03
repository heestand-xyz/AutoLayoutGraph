//
//  ALGNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGNode_hpp
#define ALGNode_hpp

#include <uuid/uuid.h>
#include <string>
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
    uuid_t id;
    string typeName;
    ALGPosition position;
    
    ALGGroupNode* parent = nullptr;
    
    vector<ALGWire*> inputWires;
    vector<ALGWire*> outputWires;
    
    ALGNode(string typeName);
    virtual ~ALGNode() {}
    
    virtual ALGSize size(ALGLayout layout) = 0;
    bool hitTest(ALGPoint point, ALGLayout layout);
    
    static void connect(ALGNode* leadingNode, ALGNode* trailingNode);
    static void disconnect(ALGNode* leadingNode, ALGNode* trailingNode);
    static void connect(ALGWire*);
    static void disconnect(ALGWire*);
    
    static ALGWire* optionalWire(ALGNode* leadingNode, ALGNode* trailingNode);
    static bool isConnected(ALGNode* leadingNode, ALGNode* trailingNode);
    static bool isLoop(ALGNode* leadingNode, ALGNode* trailingNode);
    
    bool containsDownstream(uuid_t id);
    bool containsUpstream(uuid_t id);
    
    void removeFromParent();
    ALGGroupNode* root();
    
    ALGNodeSection* section();
    
    void autoLayout(ALGLayout layout);
    void autoLayout(ALGWire* wire, ALGLayout layout);
};

#endif /* ALGNode_hpp */
