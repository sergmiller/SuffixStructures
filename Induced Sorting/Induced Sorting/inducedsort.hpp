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

//struct LMS {
//    size_t start;
//    size_t length;
//};


class InducedSorting {
public:
    static vector<size_t> getSuffArray(std::string s,
                                       size_t alpSize);
private:
    class WorkingClass {
        size_t alpSize;
        vector <size_t> str;
        vector <types> type;
        vector <vector <size_t> > basket;
        bool isEqualLMS(size_t lms1, size_t lms2);
    
        vector<size_t> suffixArrayInducedSortAlg(vector<size_t>& str,
                                                    size_t alpSize);
    
        void inducedSorting(action action, vector <size_t>& sortedData);
    //static vector <size_t> getFactorString;
    };
};

#endif /* inducedsort_hpp */
