//
//  Remove.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-11-01.
//

#ifndef Remove_hpp
#define Remove_hpp

using namespace std;

template <typename T>
void removeIn(vector<T>& vec, const T& element) {
    vec.erase(
        remove(vec.begin(), vec.end(), element),
        vec.end()
    );
}

#endif /* Remove_hpp */
