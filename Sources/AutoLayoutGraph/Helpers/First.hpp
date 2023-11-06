//
//  First.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-03.
//  

#ifndef First_hpp
#define First_hpp

using namespace std;

template<typename T, typename Predicate>
T* firstWhere(const vector<T*>& vec, Predicate pred) {
    for (const auto& elem : vec) {
        if (pred(elem)) {
            return elem;
        }
    }
    return nullptr;
}

#endif /* First_hpp */
