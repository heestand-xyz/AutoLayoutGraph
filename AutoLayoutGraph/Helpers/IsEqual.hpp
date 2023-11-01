//
//  IsEqual.hpp
//  AutoLayoutGraph
//
//  Created by Heestand, Anton Norman | Anton | GSSD on 2023-11-01.
//

#ifndef IsEqual_hpp
#define IsEqual_hpp

using namespace std;

bool isEqual(const uuid_t& uuid1, const uuid_t& uuid2) {
    return memcmp(uuid1, uuid2, sizeof(uuid_t)) == 0;
}

#endif /* IsEqual_hpp */
