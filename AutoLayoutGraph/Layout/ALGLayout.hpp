//
//  ALGLayout.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#ifndef ALGLayout_hpp
#define ALGLayout_hpp

#include "ALGPoint.hpp"
#include "ALGSize.hpp"

struct ALGLayout {
    
    int padding;
    int spacing;
    
    ALGLayout(int padding, int spacing);
};

#endif /* ALGLayout_hpp */
