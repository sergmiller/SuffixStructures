//
//  main.cpp
//  Induced Sorting
//
//  Created by Сергей Миллер on 16.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include "inducedsort.hpp"

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    std::string s;
    std::cin >> s;
    vector <size_t> sa = InducedSorting::getSuffArray(s, 26);
    return 0;
}
