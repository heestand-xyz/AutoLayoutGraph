//
//  ALGLayout.cpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#include "ALGLayout.hpp"

ALGLayout::ALGLayout(double padding, double spacing) {
    this->padding = padding;
    this->spacing = spacing;
}

// MARK: - Description

ostream& operator<<(ostream& os, const ALGLayout layout) {
    os << "layout(padding: " << layout.padding << ", spacing: " << layout.spacing << ")";
    return os;
}

string ALGLayout::description() {
    return "layout(padding: " + to_string(padding) + ", spacing: " + to_string(spacing) + ")";
}
