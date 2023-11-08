//
//  ALGRect.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGRect_hpp
#define ALGRect_hpp

#include <iostream>
#include "ALGPoint.hpp"
#include "ALGSize.hpp"

using namespace std;

struct ALGRect {
    
    ALGPoint origin;
    ALGSize size;
    
    ALGRect(ALGPoint origin, ALGSize size);
    
    ALGRect padding(double padding);
    
    friend ostream& operator<<(ostream& os, const ALGRect& rect);
    string description();
};

#endif /* ALGRect_hpp */
