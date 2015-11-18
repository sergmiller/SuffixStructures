//
//  inducedsort.hpp
//  Induced Sorting
//
//  Created by Сергей Миллер on 16.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#ifndef inducedsort_hpp
#define inducedsort_hpp

#include <stdio.h>
#include <string>
#include <vector>

using std::vector;

enum types{
    L_TYPE,
    S_TYPE
};

enum action{
    SORT_LMS_SUBSTR,
    GET_SUFF_ARRAY
};

class InducedSorting {
public:
    static vector<size_t> getSuffArray(std::string s, size_t alpSize);
private:
    static vector<size_t> suffixArrayInducedSortAlg(vector<size_t>& s, size_t alpSize);
    static void inducedSorting(action action, vector<size_t>& str, vector <size_t>& sortedData, vector <size_t>& bucket);
};

#endif /* inducedsort_hpp */
