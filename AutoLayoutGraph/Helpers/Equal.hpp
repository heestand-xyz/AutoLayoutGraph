//
//  Equal.hpp
//  AutoLayoutGraph
//
//  Created by Anton Heestand on 2023-11-01.
//

#ifndef Equal_hpp
#define Equal_hpp

using namespace std;

inline bool isEqual(const uuid_t& uuid1, const uuid_t& uuid2) {
    return memcmp(uuid1, uuid2, sizeof(uuid_t)) == 0;
}

#endif /* Equal_hpp */
