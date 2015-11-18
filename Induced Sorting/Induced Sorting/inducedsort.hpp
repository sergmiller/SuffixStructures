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
    public:
        WorkingClass(vector <size_t>& str, size_t alpSize);
        vector <size_t> suffixArrayInducedSortAlg();
    private:
        size_t alpSize;
        vector <types> type;
        vector <size_t>& str;
        vector <size_t> lmsSubstr;
        vector <bool> lmsCharFlag;
        vector <size_t> character;
        vector <vector <size_t> > basket;
        
        void getTypes();
        void setUpBasket();
        void clearBasket();
        void getLMSCharacters();
        void calcFactorStrings();
        void induceSuffixArray(vector <size_t> inducedSuffixArray);
        bool isEqualLMS(size_t lms1, size_t lms2);
        void inducedSorting(action action, vector <size_t>& sortedData);
    //static vector <size_t> getFactorString;
    };
};

#endif /* inducedsort_hpp */
