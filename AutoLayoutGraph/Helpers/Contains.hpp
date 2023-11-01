//
//  Contains.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-11-01.
//

#ifndef Contains_hpp
#define Contains_hpp

using namespace std;

template<typename T, typename Predicate>
bool containsWhere(const vector<T>& vec, Predicate pred) {
    for (const auto& elem : vec) {
        if (pred(elem)) {
            return true;
        }
    }
    return false;
}

#endif /* Contains_hpp */
