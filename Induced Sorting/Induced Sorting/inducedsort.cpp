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


vector <size_t> InducedSorting::getSuffArray(std::string s,
                                             size_t alpSize) {
    vector <size_t> str(s.size() + 1);
    for(size_t i  = 0;i < s.size(); ++i) {
        str[i] = (s[i] - 'a') + 1;
    }
    str[s.size()] = 0;
    
    return suffixArrayInducedSortAlg(str, alpSize + 1);
}

vector <size_t> InducedSorting::suffixArrayInducedSortAlg(vector <size_t>& str,
                                                          size_t alpSize) {
    vector <types> type(str.size());
    vector <bool> lmsCharFlag(str.size(), false);
    vector <size_t> lmsSubstr;
    
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
            lmsCharFlag[i + 1 ] = true;
        }
    }
    
    
    vector <size_t> inducedStr(lmsSubstr.size());
    vector <vector <size_t> > basket(alpSize);
    
    //calc basket size
    vector <size_t> countChars(alpSize, 0);
    
    for(size_t i = 0;i < str.size(); ++i) {
        ++countChars[str[i]];
    }
    
    //set up basket size
    for(size_t i = 0;i < alpSize; ++i) {
        basket[i].resize(countChars[i], SIZE_T_MAX);
        cout << countChars[i] << std::endl;
    }
    
    
    inducedSorting(SORT_LMS_SUBSTR, str, lmsSubstr, type, basket);
    
    
    
    //get characters for induction step
    vector <size_t> character(str.size());
    
    size_t lastChar = 0;
    size_t lastLms = 0;
    bool existPrev = false;
    
    for(size_t i = 0;i < basket.size(); ++i) {
        for(size_t j = 0;j < basket[i].size(); ++j) {
            if(lmsCharFlag[basket[i][j]]) {
                if(existPrev) {
                    if(isEqualLMS(lastLms, basket[i][j], str, type)) {
                        character[basket[i][j]] = lastChar;
                    } else {
                        character[basket[i][j]] = ++lastChar;
                    }
                } else {
                    character[basket[i][j]] = 0;
                }
                
                lastLms = basket[i][j];
                existPrev = true;
            }
        }
    }
    
    vector <size_t> str1;// = getFactorString(basket, lmsCharFlag, str, type); (lmsSubstr.size());
    
    for(size_t i = 0;i < str1.size(); ++i) {
        str1[i] = character[lmsSubstr[i]];
    }

//    cout << "S1:\n";
//    
//    for(size_t i = 0;i < str1.size(); ++i) {
//        cout << str1[i] << " ";
//    }
//    cout << "\n";
//    
//    cout << "sorted LMS-prefix:\n";
//    for(size_t i = 0;i < basket.size(); ++i) {
//        cout << "{ ";
//        for(size_t j = 0;j < basket[i].size(); ++j) {
//            if(basket[i][j] == SIZE_T_MAX) {
//                cout << "-1 ";
//            } else {
//                cout << basket[i][j] << " ";
//            }
//        }
//        cout << "} ";
//    }
//    cout << "\n";
    

    
    return *(new std::vector <size_t>);
}

void InducedSorting::inducedSorting(action action,
                                    vector<size_t>& str,
                                    vector <size_t>& sortedData,
                                    vector <types>& type,
                                    vector <vector <size_t> >& basket) {
    vector <size_t> head(basket.size(), 0);//used twice: for insert LMS-suffix and for induced sort
    vector <size_t> tail(basket.size());

    //Step 1
    if(action == SORT_LMS_SUBSTR) {
        //insert LMS substrings
        vector <size_t>& lmsSuff = sortedData;
        for(size_t i = 0;i < lmsSuff.size(); ++i) {
            size_t curBasket = str[lmsSuff[i]];
            basket[curBasket][basket[curBasket].size() - 1 - head[curBasket]] = lmsSuff[i];
            ++head[curBasket];
        }
        
        for(size_t i = 0;i < basket.size();++i) {
            tail[i] = basket[i].size() - 1;
            head[i] = 0;
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
                basket[curChar][head[curChar]] = curPos;
                ++head[curChar];
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
                basket[curChar][tail[curChar]] = curPos;
                --tail[curChar];
            }
        }
    }
}

bool InducedSorting::isEqualLMS(size_t lms1,
                                size_t lms2,
                                vector <size_t>& str,
                                vector <types>& type) {
    bool existLType = false;
    while(lms1 < str.size() && lms2 < str.size() && str[lms1] == str[lms2] && type[lms1] == type[lms2]) {
        if(type[lms1] == L_TYPE) {
            existLType = true;
        }
        
        if(existLType && type[lms1] == S_TYPE) {
            return true;
        }

        ++lms1;
        ++lms2;
    }
    return false;
}
