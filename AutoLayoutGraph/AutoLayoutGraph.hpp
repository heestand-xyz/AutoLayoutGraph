//
//  AutoLayoutGraph.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef AutoLayoutGraph_
#define AutoLayoutGraph_

#pragma GCC visibility push(default)

#include "Nodes/ALGNode.hpp"
#include "Nodes/ALGItemNode.hpp"
#include "Nodes/ALGGroupNode.hpp"
#include "Layout/ALGLayout.hpp"

using namespace std;

class AutoLayoutGraph {
    
    ALGLayout layout;
    
    AutoLayoutGraph(ALGLayout layout);
};

#pragma GCC visibility pop
#endif
