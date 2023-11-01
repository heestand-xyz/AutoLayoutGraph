//
//  ALGPoint.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#ifndef ALGPoint_hpp
#define ALGPoint_hpp

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
};

#endif /* ALGPoint_hpp */
