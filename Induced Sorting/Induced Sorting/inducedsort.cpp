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
#include <iostream>

using std::cout;


vector <size_t> InducedSorting::getSuffArray(std::string s, size_t alpSize) {
    vector <size_t> str(s.size() + 1);
    for(size_t i  = 0;i < s.size(); ++i) {
        str[i] = (s[i] - 'a') + 1;
    }
    str[s.size()] = 0;
    
    return suffixArrayInducedSortAlg(str, alpSize + 1);
}

vector <size_t> InducedSorting::suffixArrayInducedSortAlg(vector <size_t>& str, size_t alpSize) {
    std::vector <types> type(str.size());
    std::vector <size_t> lmsSubstr;
    
    type[str.size() - 1] = S_TYPE;
    
    size_t i = str.size() - 1;
    while(i != 0) {
        --i;
        if(str[i] < str[i + 1]) {
            type[i] = S_TYPE;
        }
        
        if(str[i] > str[i + 1]) {
            type[i] = L_TYPE;
        }
        
        if(str[i] == str[i + 1]) {
        type[i] = type[i + 1];
        }
    }
    
    for(size_t i = 0;i < str.size() - 1; ++i) {
        if(type[i] == L_TYPE && type[i + 1] == S_TYPE) {
            lmsSubstr.push_back(i + 1);
        }
    }
    
    
    vector <size_t> inducedStr(lmsSubstr.size());
    vector <vector <size_t> > basket(alpSize);
    
    //calc basket size
    vector <size_t> countChars(alpSize, 0);
    
    for(size_t i = 0;i < str.size(); ++i) {
        ++countChars[str[i]];
    }
    
    //establish basket size
    for(size_t i = 0;i < alpSize; ++i) {
        basket[i].resize(countChars[i], SIZE_T_MAX);
        cout << countChars[i] << std::endl;
    }
    
    
    inducedSorting(SORT_LMS_SUBSTR, str, lmsSubstr, type, basket);
    
    
    
    cout << "sorted LMS-prefix:\n";
    for(size_t i = 0;i < basket.size(); ++i) {
        cout << "{ ";
        for(size_t j = 0;j < basket[i].size(); ++j) {
            if(basket[i][j] == SIZE_T_MAX) {
                cout << "-1 ";
            } else {
                cout << basket[i][j] << " ";
            }
        }
        cout << "} ";
    }
    cout << "\n";
    

    
    return *(new std::vector <size_t>);
}

void InducedSorting::inducedSorting(action action, vector<size_t>& str, vector <size_t>& sortedData, vector <types>& type, vector <vector <size_t> >& basket) {
    vector <size_t> first(basket.size(), 0);//used twice: for insert LMS-suffix and for induced sort
    vector <size_t> last(basket.size());

    //Step 1
    if(action == SORT_LMS_SUBSTR) {
        //insert LMS substrings
        vector <size_t>& lmsSuff = sortedData;
        for(size_t i = 0;i < lmsSuff.size(); ++i) {
            size_t curBasket = str[lmsSuff[i]];
            basket[curBasket][basket[curBasket].size() - 1 - first[curBasket]] = lmsSuff[i];
            ++first[curBasket];
        }
        
        for(size_t i = 0;i < basket.size();++i) {
            last[i] = basket[i].size() - 1;
            first[i] = 0;
        }
    } else {
        //TO DO
        //GET SUFF ARRAY
    }
    
    //Step 2: insert L-type LMS-prefix
    for(size_t i = 0;i < basket.size(); ++i) {
        for(size_t j = 0;j < basket[i].size(); ++j) {
            if(basket[i][j] == SIZE_T_MAX || basket[i][j] == 0) {
                continue;
            }
            
            size_t curPos = basket[i][j] - 1;
            size_t curChar = str[curPos];
            if(type[curPos] == L_TYPE) {
                basket[curChar][first[curChar]] = curPos;
                ++first[curChar];
            }
        }
    }
    
    
    //Step 3: insert S-type LMS-prefix
    size_t i = basket.size();
    while(i != 0) {
        --i;
        size_t j = basket[i].size();
        while(j != 0) {
            --j;
            if(basket[i][j] == SIZE_T_MAX || basket[i][j] == 0) {
                continue;
            }
            
            size_t curPos = basket[i][j] - 1;
            size_t curChar = str[curPos];
            if(type[curPos] == S_TYPE) {
                basket[curChar][last[curChar]] = curPos;
                --last[curChar];
            }
        }
    }
}




