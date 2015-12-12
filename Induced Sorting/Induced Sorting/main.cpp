//
//  main.cpp
//  Induced Sorting
//
//  Created by Сергей Миллер on 16.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "inducedsort.hpp"

using std::cin;
using std::cout;
using std::string;
using std::vector;

int main() {
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    string s;
    cin >> s;
    vector <size_t> sa = InducedSorting::getSuffArray(s);
    for(size_t i = 0;i < sa.size(); ++i) {
        cout << sa[i] << " ";
    }
    cout << "\n";
    vector <size_t> lcp = getLcpKasai(s, sa);
    for(size_t i = 0;i < lcp.size(); ++i) {
        cout << lcp[i] << " ";
    }
    
    return 0;
}

