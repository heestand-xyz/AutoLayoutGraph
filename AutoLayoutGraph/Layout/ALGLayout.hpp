//
//  ALGLayout.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGLayout_hpp
#define ALGLayout_hpp

#include "ALGPoint.hpp"
#include "ALGSize.hpp"

struct ALGLayout {
    
    double padding;
    double spacing;
    
    ALGLayout(double padding, double spacing);
};

#endif /* ALGLayout_hpp */
