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
    
    int x;
    int y;
    
    ALGPoint(int x, int y);
    
    static const ALGPoint zero;
    
    ALGPoint operator+(ALGPoint point);
    ALGPoint operator-(ALGPoint point);
    ALGPoint operator*(ALGPoint point);
    ALGPoint operator/(ALGPoint point);
    ALGPoint operator+(int point);
    ALGPoint operator-(int point);
    ALGPoint operator*(int point);
    ALGPoint operator/(int point);
    
    friend ostream& operator<<(ostream& os, const ALGPoint& point);
};

#endif /* ALGPoint_hpp */
