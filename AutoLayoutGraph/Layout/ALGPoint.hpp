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
};

#endif /* ALGPoint_hpp */
