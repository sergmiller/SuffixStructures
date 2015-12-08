//
//  ukkonen.hpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory.h>

#ifndef ukkonen_hpp
#define ukkonen_hpp

using std::string;


#define MAX_SIZE 2002
//#define size_t int
#define nullptr NULL

typedef unsigned long long ui64;


class SuffixTree {
public:
    void buildTree(std::string& s);
    //bool findString(std::string& s);
    long long calcSum();
    //~SuffixTree();
private:
    class Node {
    public:
        Node (size_t startPos, size_t length, ui64 parent, size_t index);//make cutting vert
        Node (size_t* lastPos, ui64 parent, size_t index);//make leaf
        bool isLeaf;
        size_t startPos;
        size_t index;
        size_t getLength();
        size_t length;
        size_t* lastPos;//pointer to last char in str
        ui64 next[27];
        ui64 parent;
        ui64 link;
    };
    
    class UkkonenBuilder {
    public:
        UkkonenBuilder(std::string& s, SuffixTree& suffTree);
        bool emptyPhase;
        size_t lastChar;
        size_t firstPos;
        size_t currentOffset;
        Node addLeaf(ui64 cutVertNumber);
        void addIntoTree(char c);
        void addNewLeafs();
        void initNewVerticies();
        void normolizeOffsetLength();
        void continueTree();
        void addBranchFromVert();
        void addBranchBetweenVert();
        ui64 currentPoint;
        ui64 prevLinkPoint;
        ui64 currentLinkPoint;
        
        std::string& str;
        SuffixTree& suffTree;
        std::string getEdge(std::string& s, size_t start, size_t len);
        std::vector <Node>& tree;
    };
    
    size_t lastPos;
    size_t root;
    size_t dummy;
    
    std::vector <Node> tree;
};

#endif /* ukkonen_hpp */