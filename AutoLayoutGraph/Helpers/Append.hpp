//
//  Append.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#ifndef Append_hpp
#define Append_hpp

using namespace std;

template <typename T>
vector<T>& operator +=(vector<T>& a, const vector<T>& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

#endif /* Append_hpp */
