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
    
    void reset();
    void temporaryOrigin(ALGPoint origin);
    void finalizeOrigin(ALGPoint origin);
    
    friend ostream& operator<<(ostream& os, const ALGPosition position);
    string description();
    string stateDescription();
};

#endif /* ALGPosition_hpp */
