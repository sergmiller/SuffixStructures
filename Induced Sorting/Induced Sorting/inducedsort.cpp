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

vector <size_t> getLcpKasai(const std::string& s, const vector <size_t>& suffArray) {
    vector <size_t> lcp(s.size() + 1);
    vector <size_t> pos(s.size() + 1);
    
    for(size_t i = 0;i <= s.size(); ++i) {
        pos[suffArray[i]] = i;
    }
    
    size_t k = 0;
    for(size_t i = 0;i <= s.size(); ++i) {
        if(k > 0) {
            --k;
        }
        
        if(pos[i] == s.size()) {
            lcp[s.size()] = SIZE_T_MAX;
            k = 0;
        } else {
            size_t j = suffArray[pos[i] + 1];
            while(std::max(i + k, j + k) < s.size() && s[i + k] == s[j + k]) {
                ++k;
            }
            lcp[pos[i]] = k;
        }
    }
    return lcp;
}

vector <size_t> InducedSorting::getSuffArray(const std::string& s) {
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

InducedSorting::WorkingClass::WorkingClass(const vector <size_t>& str, size_t alpSize):_str(str) {
    WorkingClass::_alpSize = alpSize;
}

void InducedSorting::WorkingClass::_getTypes() {
    _type.resize(_str.size());
    _type[_str.size() - 1] = S_TYPE;
    
    size_t i = _str.size() - 1;
    while(i != 0) {
        --i;
        if(_str[i] < _str[i + 1]) {
            _type[i] = S_TYPE;
        }
        
        if(_str[i] > _str[i + 1]) {
            _type[i] = L_TYPE;
        }
        
        if(_str[i] == _str[i + 1]) {
            _type[i] = _type[i + 1];
        }
    }
}

void InducedSorting::WorkingClass::_getLMSCharacters() {
    _lmsCharFlag.resize(_str.size(), false);
    
    //case when the sentinel is the one LMS-character
    if(_str.size() == 1) {
        _lmsSubstr.push_back(0);
        _lmsCharFlag[0] = true;
        return;
    }
    
    for(size_t i = 0;i < _str.size() - 1; ++i) {
        if(_type[i] == L_TYPE && _type[i + 1] == S_TYPE) {
            _lmsSubstr.push_back(i + 1);
            _lmsCharFlag[i + 1] = true;
        }
    }
}

void InducedSorting::WorkingClass::_setUpBaskets() {
    _baskets.resize(_alpSize);
    
    //calc basket size
    vector <size_t> countChars(_alpSize, 0);
    
    for(size_t i = 0;i < _str.size(); ++i) {
        ++countChars[_str[i]];
    }
    
    //set up basket size
    for(size_t i = 0;i < _alpSize; ++i) {
        _baskets[i].resize(countChars[i], SIZE_T_MAX);
    }
}

//get characters for induced string
void InducedSorting::WorkingClass::_calcFactorStrings() {
    _character.resize(_str.size());
    
    _countChar = 0;
    size_t lastLms = 0;
    bool existPrev = false;
    
    for(size_t i = 0;i < _baskets.size(); ++i) {
        for(size_t j = 0;j < _baskets[i].size(); ++j) {
            if(_lmsCharFlag[_baskets[i][j]]) {
                if(existPrev) {
                    if(_isEqualLMS(lastLms, _baskets[i][j])) {
                        _character[_baskets[i][j]] = _countChar;
                    } else {
                        _character[_baskets[i][j]] = ++_countChar;
                        _factorStr.push_back(_baskets[i][j]);
                    }
                } else {
                    _character[_baskets[i][j]] = 0;
                    _factorStr.push_back(_baskets[i][j]);
                }
                
                lastLms = _baskets[i][j];
                existPrev = true;
            }
        }
    }
    
    _inducedStr.resize(_lmsSubstr.size());
    
    for(size_t i = 0;i < _inducedStr.size(); ++i) {
        
        _inducedStr[i] = _character[_lmsSubstr[i]];
    }
}

void InducedSorting::WorkingClass::_clearBaskets() {
    for(size_t i = 0;i < _baskets.size(); ++i) {
        for(size_t j = 0;j < _baskets[i].size(); ++j) {
            _baskets[i][j] = SIZE_T_MAX;
        }
    }
}

void InducedSorting::WorkingClass::_directlyComputeInducedSuffixArray() {
    _inducedSuffixArray.resize(_inducedStr.size());
    for(size_t i = 0;i < _inducedStr.size(); ++i) {
        _inducedSuffixArray[_inducedStr[i]] = i;
    }
}

void InducedSorting::WorkingClass::_inductionStep() {
    if(_countChar + 1 == _lmsSubstr.size()) {
        _directlyComputeInducedSuffixArray();
    } else {
        
        WorkingClass induction = WorkingClass(_inducedStr, _alpSize);
        _inducedSuffixArray = induction.suffixArrayInducedSortAlg();
    }
}

vector <size_t> InducedSorting::WorkingClass::suffixArrayInducedSortAlg() {
    
    _getTypes();
    _getLMSCharacters();
    _setUpBaskets();
    
    _inducedSorting(SORT_LMS_SUBSTR, _lmsSubstr);
    _calcFactorStrings();
    
    _inductionStep();
    
    _clearBaskets();
    
    _inducedSorting(GET_SUFF_ARRAY, _inducedSuffixArray);
    
    _updateSuffArrayFromBasket();
    
    return _suffixArray;
}

void InducedSorting::WorkingClass::_updateSuffArrayFromBasket() {
    for(size_t i = 0;i < _baskets.size(); ++i) {
        for(size_t j = 0;j < _baskets[i].size(); ++j) {
            _suffixArray.push_back(_baskets[i][j]);
        }
    }
}

void InducedSorting::WorkingClass::_insertLMSSubstringsInBaskets(const vector <size_t>& lmsSuff) {
    //insert LMS substrings
    vector <size_t> offset(_baskets.size(), 0);
    for(size_t i = 0;i < lmsSuff.size(); ++i) {
        size_t curBasket = _str[lmsSuff[i]];
        _baskets[curBasket][_baskets[curBasket].size() - 1 - offset[curBasket]] = lmsSuff[i];
        ++offset[curBasket];
    }
}

void InducedSorting::WorkingClass::_insertInducedSuffixArrayInBaskets(const vector <size_t>& inducedSuffArray) {
    //get suffix array from induced suffix array
    vector <size_t> offset(_baskets.size(), 0);
    
    size_t i = inducedSuffArray.size();
    while (i != 0) {
        --i;
        size_t curBasket = _str[_lmsSubstr[inducedSuffArray[i]]];
        _baskets[curBasket][_baskets[curBasket].size() - 1 - offset[curBasket]] = _lmsSubstr[inducedSuffArray[i]];
        ++offset[curBasket];
    }
}

void InducedSorting::WorkingClass::_insertLTypeLMSprefix() {
    vector <size_t> head(_baskets.size(), 0);
    for(size_t i = 0;i < _baskets.size(); ++i) {
        for(size_t j = 0;j < _baskets[i].size(); ++j) {
            if(_baskets[i][j] == SIZE_T_MAX || _baskets[i][j] == 0) {
                continue;
            }
            
            size_t curPos = _baskets[i][j] - 1;
            size_t curChar = _str[curPos];
            if(_type[curPos] == L_TYPE) {
                _baskets[curChar][head[curChar]] = curPos;
                ++head[curChar];
            }
        }
    }
}

void InducedSorting::WorkingClass::_insertSTypeLMSprefix() {
    vector <size_t> tail(_baskets.size());
    size_t i = 0;
    for(;i < _baskets.size();++i) {
        tail[i] = _baskets[i].size() - 1;
    }
    
    i = _baskets.size();
    
    while(i != 0) {
        --i;
        size_t j = _baskets[i].size();
        while(j != 0) {
            --j;
            if(_baskets[i][j] == SIZE_T_MAX || _baskets[i][j] == 0) {
                continue;
            }
            
            size_t curPos = _baskets[i][j] - 1;
            size_t curChar = _str[curPos];
            if(_type[curPos] == S_TYPE) {
                _baskets[curChar][tail[curChar]] = curPos;
                --tail[curChar];
            }
        }
    }

}

void InducedSorting::WorkingClass::_inducedSorting(_ACTION action, const vector <size_t>& sortedData) {
    //used twice: for insert LMS-suffix/induced suffix array  and for induced sort

    //Step 1 init
    if(action == SORT_LMS_SUBSTR) {
        _insertLMSSubstringsInBaskets(sortedData);
    } else {
        _insertInducedSuffixArrayInBaskets(sortedData);
    }
    
    //Step 2: insert L-type LMS-prefix
    _insertLTypeLMSprefix();
    
    //Step 3: insert S-type LMS-prefix
    _insertSTypeLMSprefix();
}

bool InducedSorting::WorkingClass::_isEqualLMS(size_t lms1, size_t lms2) {
    bool existLType = false;
    while(lms1 < _str.size() && lms2 < _str.size() && _str[lms1] == _str[lms2] && _type[lms1] == _type[lms2]) {
        if(_type[lms1] == L_TYPE) {
            existLType = true;
        }
        
        if(existLType && _type[lms1] == S_TYPE) {
            return true;
        }

        ++lms1;
        ++lms2;
    }
    return false;
}
