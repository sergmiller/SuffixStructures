//
//  inducedsort.cpp
//  Induced Sorting
//
//  Created by Сергей Миллер on 16.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include "inducedsort.hpp"
#include <vector>
#include <string>


vector <size_t> InducedSorting::getSuffArray(std::string s, size_t alpSize) {
    vector <size_t> str(s.size() + 1);
    for(size_t i  = 0;i < s.size(); ++i) {
        str[i] = s[i];
    }
    str[s.size()] = 0;
    
    return suffixArrayInducedSortAlg(str, alpSize);
}

vector <size_t> InducedSorting::suffixArrayInducedSortAlg(vector <size_t>& str, size_t alpSize) {
    std::vector <types> type(str.size());
    std::vector <size_t> lmsSubstr;
    
    type[str.size() - 1] = S_TYPE;
    
    size_t i = str.size() - 1;
    while(i-- > 0) {
        if(str[i] < str[i + 1]) {
            type[i] = S_TYPE;
        }
        
        if(str[i] > str[i + 1]) {
            type[i] = L_TYPE;
        }
        
        type[i] = type[i + 1];
        
        if(type[i] == L_TYPE && type[i + 1] == S_TYPE) {
            lmsSubstr.push_back(i);
        }
    }
    
    std::reverse(lmsSubstr.begin(), lmsSubstr.end());
    
    vector <size_t> inducedStr(lmsSubstr.size());
    vector <size_t> bucket(alpSize);
    
    inducedSorting(SORT_LMS_SUBSTR, str, lmsSubstr, bucket);
    
           
    return *(new std::vector <size_t>);
}

void InducedSorting::inducedSorting(action action, vector<size_t>& str, vector <size_t>& sortedData, vector <size_t>& bucket) {
    if(action == SORT_LMS_SUBSTR) {
        
    }
}





















