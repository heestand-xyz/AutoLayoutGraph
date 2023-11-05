//
//  ALGWireLength.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-01.
//

#ifndef ALGWireLength_hpp
#define ALGWireLength_hpp

enum class ALGPositionState : int {
    NONE,
    TEMP,
    FINAL
};

inline std::ostream& operator<<(std::ostream& os, ALGPositionState state) {
    switch (state) {
        case ALGPositionState::NONE:
            os << "NONE";
            break;
        case ALGPositionState::TEMP:
            os << "TEMP";
            break;
        case ALGPositionState::FINAL:
            os << "FINAL";
            break;
        default:
            os << "UNKNOWN";
            break;
    }
    return os;
}

#endif /* ALGWireLength_hpp */
