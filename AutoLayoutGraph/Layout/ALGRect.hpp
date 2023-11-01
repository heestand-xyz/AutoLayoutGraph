//
//  ALGRect.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#ifndef ALGRect_hpp
#define ALGRect_hpp

#include "ALGPoint.hpp"
#include "ALGSize.hpp"

struct ALGRect {
    
    ALGPoint origin;
    ALGSize size;
    
    ALGRect(ALGPoint origin, ALGSize size);
    
    ALGRect padding(int padding);
};

#endif /* ALGRect_hpp */
