//
//  ALGSize.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-10-31.
//

#ifndef ALGSize_hpp
#define ALGSize_hpp

struct ALGSize {
    
    int width;
    int height;
    
    ALGSize(int width, int height);
    
    static const ALGSize zero;
    static const ALGSize one;
    
    ALGSize padding(int padding);
    
    ALGSize operator+(ALGSize point);
    ALGSize operator-(ALGSize point);
    ALGSize operator*(ALGSize point);
    ALGSize operator/(ALGSize point);
    ALGSize operator+(int point);
    ALGSize operator-(int point);
    ALGSize operator*(int point);
    ALGSize operator/(int point);
};

#endif /* ALGSize_hpp */
