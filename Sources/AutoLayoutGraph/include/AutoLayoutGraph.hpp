//
//  AutoLayoutGraph.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef AutoLayoutGraph_
#define AutoLayoutGraph_

#pragma GCC visibility push(default)

#include "../Nodes/ALGNode.hpp"
#include "../Nodes/ALGItemNode.hpp"
#include "../Nodes/ALGGroupNode.hpp"
#include "../Nodes/ALGNodeSection.hpp"
#include "../Layout/ALGLayout.hpp"

using namespace std;

class AutoLayoutGraph {
    
    ALGLayout layout;
    void disconnectWire(ALGWire* wire);
    
public:
    
    AutoLayoutGraph(ALGLayout layout);
    
    void addToParent(ALGGroupNode* parentNode, ALGNode* node);
    void moveToNewParent(ALGGroupNode* parentNode, ALGNode* node);
    void removeFromParent(ALGNode* node);
    
    void connectWire(ALGNode* leadingNode, ALGNode* trailingNode);
    void disconnectWire(ALGNode* leadingNode, ALGNode* trailingNode);
    
    static ALGWire* optionalWire(ALGNode* leadingNode, ALGNode* trailingNode);
    static bool isConnected(ALGNode* leadingNode, ALGNode* trailingNode);
    static bool isLoop(ALGNode* leadingNode, ALGNode* trailingNode);
};

#pragma GCC visibility pop
#endif
