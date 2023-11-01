//
//  ALGRect.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGRect.hpp"

ALGRect::ALGRect(ALGPoint origin, ALGSize size)
: origin(origin), size(size)
{ }

ALGRect ALGRect::padding(int padding) {
    return ALGRect(origin - padding, size.padding(padding));
}
