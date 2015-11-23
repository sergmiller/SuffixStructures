//
//  ukkonen.hpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#ifndef ukkonen_hpp
#define ukkonen_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

class UkkonenSuffixTree {
public:
    void buildTree(std::string& s);
    bool findString(std::string& s);
private:
    bool emptyPhase;
    char lastChar;
    size_t* lastPos;
    size_t sizeStr;
    size_t lastLiftUp;
    size_t numOfLastChangedSuffix;
    void addIntoTree(char c);
    void addNewLeafs();
    class Node {
    public:
        Node(size_t startPos, size_t length);
        size_t startPos;
        size_t length;//length = 0 <=> node is leaf
        Node* link;
        Node* parent;
        std::map <char, Node*> next;
    };
    
    std::vector <Node> tree;
    Node* root;
    Node* lastChangedSuffix;
    void continueTree(Node* nextSuffParent);

};

#endif /* ukkonen_hpp */
