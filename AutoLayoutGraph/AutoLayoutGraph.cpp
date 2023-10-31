//
//  AutoLayoutGraph.cpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
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

