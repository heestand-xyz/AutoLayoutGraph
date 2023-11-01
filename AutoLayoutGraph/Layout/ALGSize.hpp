//
//  ALGSize.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-10-31.
//

#ifndef ALGSize_hpp
#define ALGSize_hpp

#include <iostream>

using namespace std;

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
    
    friend ostream& operator<<(ostream& os, const ALGSize& size);
};

#endif /* ALGSize_hpp */
