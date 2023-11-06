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
    switch (static_cast<int>(state)) {
        case static_cast<int>(ALGPositionState::NONE):
            os << "NONE";
            break;
        case static_cast<int>(ALGPositionState::TEMP):
            os << "TEMP";
            break;
        case static_cast<int>(ALGPositionState::FINAL):
            os << "FINAL";
            break;
        default:
            os << "UNKNOWN";
            break;
    }
    return os;
}

#endif /* ALGWireLength_hpp */
