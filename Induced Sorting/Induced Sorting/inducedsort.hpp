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

vector <size_t> getLcpKasai(const std::string& s,const vector <size_t>& lcp);

class InducedSorting {
public:
    static vector<size_t> getSuffArray(const std::string& s);
private:
    class WorkingClass {
    public:
        WorkingClass(const vector <size_t>& str, const size_t alpSize);
        vector <size_t> suffixArrayInducedSortAlg();
    private:
        enum _ETS_LMS_SUBSTRINGS{
            L_TYPE,
            S_TYPE
        };
        
        enum _ACTION{
            SORT_LMS_SUBSTR,
            GET_SUFF_ARRAY
        };
        
        size_t _alpSize;
        size_t _countChar;
        bool _uniqueCharsFlag;
        
        vector <_ETS_LMS_SUBSTRINGS> _type;
        
        vector <bool> _lmsCharFlag;
        
        const vector <size_t>& _str;
        
        vector <size_t> _lmsSubstr;
        vector <size_t> _character;
        vector <size_t> _factorStr;
        vector <size_t> _inducedStr;
        vector <size_t> _suffixArray;
        vector <size_t> _inducedSuffixArray;
        
        vector <vector <size_t> > _baskets;
        
        void _getTypes();
        void _setUpBaskets();
        void _clearBaskets();
        void _inductionStep();
        void _getLMSCharacters();
        void _calcFactorStrings();
        void _updateSuffArrayFromBasket();
        void _directlyComputeInducedSuffixArray();
        void _insertLTypeLMSprefix();
        void _insertSTypeLMSprefix();
        void _insertLMSSubstringsInBaskets(const vector <size_t>& lmsSuff);
        void _insertInducedSuffixArrayInBaskets(const vector <size_t>& inducedSuffArray);
        void _induceSuffixArray(vector <size_t> inducedSuffixArray);
        bool _isEqualLMS(size_t lms1, size_t lms2);
        void _inducedSorting(_ACTION action, const vector <size_t>& sortedData);
    };
};

#endif /* inducedsort_hpp */
