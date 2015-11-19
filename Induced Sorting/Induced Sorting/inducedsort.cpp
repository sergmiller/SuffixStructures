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

vector <size_t> InducedSorting::getSuffArray(std::string s) {
    vector <size_t> str(s.size() + 1);
    char minChar = CHAR_MAX;
    char maxChar = CHAR_MIN;
    for(size_t i = 0;i < s.size(); ++i) {
        minChar = std::min(minChar, s[i]);
        maxChar = std::max(maxChar, s[i]);
    }
    
    for(size_t i  = 0;i < s.size(); ++i) {
        str[i] = (s[i] - minChar) + 1;
    }
    str[s.size()] = 0;
    
    WorkingClass sort = WorkingClass(str, maxChar - minChar + 2);
    
    return sort.suffixArrayInducedSortAlg();
}

InducedSorting::WorkingClass::WorkingClass(vector <size_t>& str, size_t alpSize):str(str) {
    WorkingClass::alpSize = alpSize;
}

void InducedSorting::WorkingClass::getTypes() {
    type.resize(str.size());
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
}

void InducedSorting::WorkingClass::getLMSCharacters() {
    lmsCharFlag.resize(str.size(), false);
    
    //case when the sentinel is the one LMS-character
    if(str.size() == 1) {
        lmsSubstr.push_back(0);
        lmsCharFlag[0] = true;
        return;
    }
    
    for(size_t i = 0;i < str.size() - 1; ++i) {
        if(type[i] == L_TYPE && type[i + 1] == S_TYPE) {
            lmsSubstr.push_back(i + 1);
            lmsCharFlag[i + 1] = true;
        }
    }
}

void InducedSorting::WorkingClass::setUpBasket() {
    basket.resize(alpSize);
    
    //calc basket size
    vector <size_t> countChars(alpSize, 0);
    
    for(size_t i = 0;i < str.size(); ++i) {
        ++countChars[str[i]];
    }
    
    //set up basket size
    for(size_t i = 0;i < alpSize; ++i) {
        basket[i].resize(countChars[i], SIZE_T_MAX);
    }
}

//get characters for induced string
void InducedSorting::WorkingClass::calcFactorStrings() {
    character.resize(str.size());
    
    countChar = 0;
    size_t lastLms = 0;
    bool existPrev = false;
    
    for(size_t i = 0;i < basket.size(); ++i) {
        for(size_t j = 0;j < basket[i].size(); ++j) {
            if(lmsCharFlag[basket[i][j]]) {
                if(existPrev) {
                    if(isEqualLMS(lastLms, basket[i][j])) {
                        character[basket[i][j]] = countChar;
                    } else {
                        character[basket[i][j]] = ++countChar;
                        factorStr.push_back(basket[i][j]);
                    }
                } else {
                    character[basket[i][j]] = 0;
                    factorStr.push_back(basket[i][j]);
                }
                
                lastLms = basket[i][j];
                existPrev = true;
            }
        }
    }
}

void InducedSorting::WorkingClass::clearBasket() {
    for(size_t i = 0;i < basket.size(); ++i) {
        for(size_t j = 0;j < basket[i].size(); ++j) {
            basket[i][j] = SIZE_T_MAX;
        }
    }
}

void InducedSorting::WorkingClass::directlyComputeInducedSuffixArray() {
    inducedSuffixArray.resize(inducedStr.size());
    for(size_t i = 0;i < inducedStr.size(); ++i) {
        inducedSuffixArray[inducedStr[i]] = i;
    }
}

vector <size_t> InducedSorting::WorkingClass::suffixArrayInducedSortAlg() {
    
    getTypes();
    getLMSCharacters();
    
    setUpBasket();
    
    inducedStr.resize(lmsSubstr.size());
    
    inducedSorting(SORT_LMS_SUBSTR, lmsSubstr);
    calcFactorStrings();
    
    for(size_t i = 0;i < inducedStr.size(); ++i) {
        
        inducedStr[i] = character[lmsSubstr[i]];
    }
    
    if(countChar + 1 == lmsSubstr.size()) {
        directlyComputeInducedSuffixArray();
    } else {
        
        WorkingClass induction = WorkingClass(inducedStr, alpSize);
        inducedSuffixArray = induction.suffixArrayInducedSortAlg();
    }
    
    clearBasket();
    
    inducedSorting(GET_SUFF_ARRAY, inducedSuffixArray);
    
    vector <size_t> suffArray;
    
    for(size_t i = 0;i < basket.size(); ++i) {
        for(size_t j = 0;j < basket[i].size(); ++j) {
            suffArray.push_back(basket[i][j]);
        }
    }
    
    return suffArray;
}

void InducedSorting::WorkingClass::inducedSorting(action action, vector <size_t>& sortedData) {
    vector <size_t> head(basket.size(), 0);//used twice: for insert LMS-suffix/induced suffix array  and for induced sort
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
        
    } else {
        //get suffix array from induced suffix array
        vector <size_t>& inducedSuffArray = sortedData;
        
        size_t i = inducedSuffArray.size();
        while (i != 0) {
            --i;
            size_t curBasket = str[lmsSubstr[inducedSuffArray[i]]];
            basket[curBasket][basket[curBasket].size() - 1 - head[curBasket]] = lmsSubstr[inducedSuffArray[i]];
            ++head[curBasket];
        }
    }
    
    for(size_t i = 0;i < basket.size();++i) {
        tail[i] = basket[i].size() - 1;
        head[i] = 0;
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

bool InducedSorting::WorkingClass::isEqualLMS(size_t lms1, size_t lms2) {
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
