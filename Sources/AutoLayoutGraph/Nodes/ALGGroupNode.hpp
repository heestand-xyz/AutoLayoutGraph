//
//  ALGGroupNode.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGGroupNode_hpp
#define ALGGroupNode_hpp

#include <swift/bridging>
#include <vector>
#include "ALGNode.hpp"
#include "../Wires/ALGWire.hpp"
#include "../Layout/ALGLayout.hpp"
#include "../Layout/Types/ALGSize.hpp"

class ALGNodeSection;

using namespace std;

class ALGGroupNode : public ALGNode {
    
public:
    
    vector<ALGNodeSection*> sections;
    
    ALGGroupNode(string name, string typeName);
    
    ~ALGGroupNode() {
        cout << "deinit of group " << this << endl;
    }
    
    ALGPoint origin(ALGLayout layout);
    
    void add(ALGNode* node, ALGLayout layout);
    void remove(ALGNode* node, ALGLayout layout);
    
    vector<ALGNode*> allNodes();
    
    bool contains(ALGNode* node);
    bool deepContains(ALGNode* node);
    bool deepHitTest(ALGNode* node, ALGPoint point, ALGLayout layout);
    vector<ALGNode*> deepNodes();
        
    ALGSize size(ALGLayout layout);
    ALGSize groupSize(ALGLayout layout) SWIFT_RETURNS_INDEPENDENT_VALUE;
    ALGRect deepFrame(ALGNode* node, ALGLayout layout);
    
    void updateSectionsOnConnect(ALGWire* wire);
    void updateSectionsOnDisconnect(ALGWire* wire);
    
    ALGNodeSection* addSection();
    void removeSection(ALGNodeSection* section);
    
    bool isRoot();
    
    void autoLayout(ALGLayout layout);
    
    ALGNode* asNode() SWIFT_RETURNS_INDEPENDENT_VALUE;
    
    friend ostream& operator<<(ostream& os, const ALGGroupNode* groupNode);
    string description();
};

#endif /* ALGGroupNode_hpp */
