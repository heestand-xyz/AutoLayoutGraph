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
    
    double width;
    double height;
    
    ALGSize(double width, double height);
    
    static const ALGSize zero;
    static const ALGSize one;
    
    ALGSize padding(double padding);
    
    ALGSize operator+(ALGSize point);
    ALGSize operator-(ALGSize point);
    ALGSize operator*(ALGSize point);
    ALGSize operator/(ALGSize point);
    ALGSize operator+(double point);
    ALGSize operator-(double point);
    ALGSize operator*(double point);
    ALGSize operator/(double point);
    
    friend ostream& operator<<(ostream& os, const ALGSize& size);
};

#endif /* ALGSize_hpp */
