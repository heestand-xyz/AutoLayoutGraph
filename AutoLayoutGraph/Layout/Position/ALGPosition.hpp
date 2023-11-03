//
//  ALGPosition.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#ifndef ALGPosition_hpp
#define ALGPosition_hpp

#include "../Types/ALGPoint.hpp"
#include "ALGPositionState.hpp"

class ALGNodeSection;

struct ALGPosition {
    
    ALGPoint origin = ALGPoint::zero;
    
    ALGPositionState state = ALGPositionState::NONE;
        
    ALGPoint originInSection(ALGNodeSection* section);
};

#endif /* ALGPosition_hpp */
