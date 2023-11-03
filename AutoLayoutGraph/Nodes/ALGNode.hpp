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
#include "../Layout/ALGPoint.hpp"
#include "../Layout/ALGSize.hpp"
#include "../Layout/ALGRect.hpp"
#include "../Layout/ALGLayout.hpp"

using namespace std;

class ALGGroupNode;
class ALGWire;

class ALGNode {
    
public:
    uuid_t id;
    string typeName;
    ALGPoint origin;
    
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
};

#endif /* ALGNode_hpp */
