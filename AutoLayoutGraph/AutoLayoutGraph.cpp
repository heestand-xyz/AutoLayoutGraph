//
//  AutoLayoutGraph.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include <iostream>
#include "AutoLayoutGraph.hpp"
#include "AutoLayoutGraphPriv.hpp"

void AutoLayoutGraph::HelloWorld(const char * s)
{
    AutoLayoutGraphPriv *theObj = new AutoLayoutGraphPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void AutoLayoutGraphPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

