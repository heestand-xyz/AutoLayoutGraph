//
//  ALGPoint.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGPoint_hpp
#define ALGPoint_hpp

#include <iostream>

using namespace std;

struct ALGPoint {
    
    double x;
    double y;
    
    ALGPoint(double x, double y);
    
    static const ALGPoint zero;
    
    ALGPoint operator+(ALGPoint point);
    ALGPoint operator-(ALGPoint point);
    ALGPoint operator*(ALGPoint point);
    ALGPoint operator/(ALGPoint point);
    ALGPoint operator+(double point);
    ALGPoint operator-(double point);
    ALGPoint operator*(double point);
    ALGPoint operator/(double point);
    
    friend ostream& operator<<(ostream& os, const ALGPoint& point);
    string description();
};

#endif /* ALGPoint_hpp */
