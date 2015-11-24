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
using std::string;

int main(int argc, const char * argv[]) {
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    string s;
    cin >> s;
    //s.push_back('$');
    SuffixTree st;
    st.buildTree(s);
    //st.outTree(st.root);
    //cout << s << std::endl;
    cout << st.calcSum() << "\n";
  
    return 0;
}
