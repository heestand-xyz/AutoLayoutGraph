//
//  ALGPosition.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#include "ALGPosition.hpp"
#include "../../Nodes/ALGNodeSection.hpp"

ALGPoint ALGPosition::originInSection(ALGNodeSection* section) {
    
    ALGPoint sectionOrigin = ALGPoint::zero;
    
    for (ALGNode* node : section->nodes) {
        sectionOrigin = ALGPoint(min(sectionOrigin.x, node->position.origin.x),
                                 min(sectionOrigin.y, node->position.origin.y));
    }
    
    return origin - sectionOrigin;
}

void ALGPosition::reset() {
    this->origin = ALGPoint::zero;
    state = ALGPositionState::NONE;
}

void ALGPosition::temporaryOrigin(ALGPoint origin) {
    this->origin = origin;
    state = ALGPositionState::TEMP;
}

void ALGPosition::finalizeOrigin(ALGPoint origin) {
    this->origin = origin;
    state = ALGPositionState::FINAL;
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGPosition position) {
    os << "position(state: " << position.state << ", origin: " << position.origin << ")";
    return os;
}

string ALGPosition::description() {
    return "position(state: " + stateDescription() + ", origin: " + origin.description() + ")";
}

string ALGPosition::stateDescription() {
    switch (static_cast<int>(state)) {
        case static_cast<int>(ALGPositionState::NONE):
            return "NONE";
            break;
        case static_cast<int>(ALGPositionState::TEMP):
            return "TEMP";
            break;
        case static_cast<int>(ALGPositionState::FINAL):
            return "FINAL";
            break;
        default:
            return "UNKNOWN";
            break;
    }
}
