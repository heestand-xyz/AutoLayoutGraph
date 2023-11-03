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
    
private:
    ALGPoint point = ALGPoint::zero;
    
public:
    ALGPositionState state = ALGPositionState::NONE;
    
    ALGPoint origin(ALGNodeSection* section);
};

#endif /* ALGPosition_hpp */
