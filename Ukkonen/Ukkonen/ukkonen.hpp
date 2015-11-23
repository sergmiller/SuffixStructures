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

class SuffixTree {
public:
    void buildTree(std::string& s);
    bool findString(std::string& s);
private:
    class Node {
    public:
        Node (size_t startPos, size_t length, size_t parent, size_t index);//make cutting vert
        Node (size_t& lastPos, size_t parent, size_t index);//make leaf
        size_t startPos;
        size_t parent;
        Node* link;
        std::map <char, Node*> next;
        size_t index;
        size_t getLength();
        size_t length;//length = 0 <=> node is leaf
        size_t* lastPos;//pointer to last char in str
    };
    
    class UkkonenBuilder {
    public:
        UkkonenBuilder(std::string& s, SuffixTree& suffTree);
        bool emptyPhase;
        char lastChar;
        size_t lastPos;
        size_t sizeStr;
        size_t lastLiftUp;
        size_t currentOffset;
        size_t numOfLastChangedSuffix;
        void addIntoTree(char c);
        void addNewLeafs();
        void continueTree();
        void addBranchFromVert();
        void addBranchBetweenVert();
        Node* prevSuffix;
        Node* nextSuffix;
        Node* currentSuffix;
        
        std::string& str;
        SuffixTree& suffTree;
    };
    
    std::vector <Node*> tree;
    Node* root;
};

#endif /* ukkonen_hpp */
