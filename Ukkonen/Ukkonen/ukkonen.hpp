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
    long long calcSum();
     ~SuffixTree();
private:
    class Node {
    private:
        Node* next[27];
    public:
        Node (size_t startPos, size_t length, Node* parent, size_t index);//make cutting vert
        Node (size_t* lastPos, Node* parent, size_t index);//make leaf
        size_t startPos;
        Node* parent;
        Node* link;
        size_t index;
        bool isLeaf;
        size_t getLength();
        size_t length;
        size_t* lastPos;//pointer to last char in str
        Node* getNext(char c);
        void setNext(char c, Node* node);
    };
    
    class UkkonenBuilder {
    public:
        UkkonenBuilder(std::string& s, SuffixTree& suffTree);
        bool emptyPhase;
        char lastChar;
        size_t firstPos;
        void addIntoTree(char c);
        void addNewLeafs();
        void normolizeOffsetLength();
        void continueTree();
        void addBranchFromVert();
        void addBranchBetweenVert();
        size_t currentOffset;
        Node* currentPoint;
        Node* prevCutPoint;
        Node* currentCutPoint;
        
        std::string& str;
        SuffixTree& suffTree;
        void outTree(Node* node);
        std::string getEdge(std::string& s, size_t start, size_t len);
    };
    
    size_t lastPos;
    std::vector <Node*> tree;
public:
    Node* root;
    Node* dummy;
};

#endif /* ukkonen_hpp */
