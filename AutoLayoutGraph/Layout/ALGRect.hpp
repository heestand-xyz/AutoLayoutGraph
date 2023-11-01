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
    
    ALGRect padding(int padding);
    
    friend ostream& operator<<(ostream& os, const ALGRect& rect);
};

#endif /* ALGRect_hpp */
