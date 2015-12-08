//
//  main.cpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include <iostream>
#include <string>
#include "ukkonen.hpp"
using std::cin;
using std::cout;


int main(int argc, const char * argv[]) {
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    string s;
    cin >> s;
    for(size_t i = 0;i < s.size(); ++i) {
        s[i] -= ('a' - 1);
    }
    SuffixTree st;
    st.buildTree(s);
    cout << st.calcSum() << "\n";
  
    return 0;
}
