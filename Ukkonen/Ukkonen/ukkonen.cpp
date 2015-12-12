//
//  ukkonen.cpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include "ukkonen.hpp"
#include <cassert>
#include <iostream>
#include <memory.h>

ui64 SuffixTree::calcSum() {
    if(!_isBuilded) {
        return -1;
    }
    ui64 sum = 0;
    for(size_t i = 2;i < _tree.size(); ++i) {
        sum += _tree[i].getLength();
    }
    return sum;
}

inline size_t SuffixTree::Node::getLength() {
    return (isLeaf ? (*lastPos - startPos) : length);
}

SuffixTree::UkkonenBuilder::UkkonenBuilder(const std::string& s, SuffixTree& suffTree):
str(s), suffTree(suffTree), tree(suffTree._tree) {
    tree.reserve(2*s.size() + 4);
    
    Node dummy = Node(0, 0, nullptr, 0);
    Node root =  Node(0, 1, 0, 1);
    tree.push_back(dummy);
    tree.push_back(root);
    
    for(size_t c = 0; c < 27; ++c) {
        tree[0].next[c] = 1;
    }
    
    tree[1].link = 0;
    
    currentOffset = 1;
    currentPoint = 1;
    prevLinkPoint = 1;
    suffTree._lastPos = 0;
}

SuffixTree::Node::Node(size_t startPos, size_t length, ui64 parent, size_t index):
startPos(startPos), length(length), parent(parent), index(index), isLeaf(false) {
    memset(next, -1, 27 * sizeof(int));
}

SuffixTree::Node::Node(size_t* lastPos, ui64 parent, size_t index):
startPos(*lastPos - 1), lastPos(lastPos), parent(parent), index(index), isLeaf(true) {
    //memset(next, 0, 27 * sizeof(Node*));
}

SuffixTree::SuffixTree(): _isBuilded(false) {};


void SuffixTree::buildTree(const std::string& s) {
    UkkonenBuilder builder(s, *this);
    for(size_t i = 0;i < s.size(); ++i) {
        builder.addIntoTree(s[i]);
    }
    _isBuilded = true;
}

inline void SuffixTree::UkkonenBuilder::addIntoTree(char c) {
    lastChar = c;
    //1 Step: increase leafs from last phase
    ++suffTree._lastPos;
    
    //2 Step: calc new nodes in tree
    addNewLeafs();
}

inline void SuffixTree::UkkonenBuilder::addNewLeafs() {
    
    emptyPhase = false;
    
    while(true) {
        normolizeOffsetLength();
        continueTree();
        
        if(prevLinkPoint != 1 && currentLinkPoint) {
            tree[prevLinkPoint].link = currentLinkPoint;
            prevLinkPoint = 1;
        }
        
        if(emptyPhase) {
            break;
        }
        
        prevLinkPoint = currentLinkPoint;
        currentLinkPoint = nullptr;
        currentPoint = tree[tree[currentPoint].parent].link;
        if(currentOffset) {
            currentPoint = tree[currentPoint].next[str[firstPos]];
        } else {
            currentOffset = tree[currentPoint].length;
        }
    }
}

inline void SuffixTree::UkkonenBuilder::normolizeOffsetLength() {
    while(currentOffset > tree[currentPoint].getLength()) {
        currentOffset -= tree[currentPoint].getLength();
        firstPos += tree[currentPoint].getLength();
        currentPoint = tree[currentPoint].next[str[firstPos]];
    }
}

void SuffixTree::UkkonenBuilder::continueTree() {
    if(currentOffset == tree[currentPoint].getLength()) {
        if(tree[currentPoint].next[lastChar] == -1) {
            addBranchFromVert();
            currentOffset = 0;
        } else {
            currentPoint = tree[currentPoint].next[lastChar];
            emptyPhase = true;
            currentOffset = 1;
        }
        currentLinkPoint = tree[currentPoint].parent;
    } else {
        if(str[tree[currentPoint].startPos + currentOffset] == lastChar) {
            ++currentOffset;
            emptyPhase  = true;
            return;
        } else {
            addBranchBetweenVert();
        }
    }
}

SuffixTree::Node SuffixTree::UkkonenBuilder::addLeaf(ui64 cutVertNumber) {
    Node newLeaf = Node(&suffTree._lastPos, cutVertNumber, tree.size());
    tree.push_back(newLeaf);
    return newLeaf;
}

void SuffixTree::UkkonenBuilder::initNewVerticies() {
    ui64 cutVertNumber = tree.size() - 1;
    
    Node& curPointer = tree[currentPoint];
    Node newLeaf =  addLeaf(cutVertNumber);
    
    curPointer.startPos += currentOffset;
    curPointer.length -= currentOffset;
    curPointer.parent = cutVertNumber;
    
    curPointer.next[str[tree[currentPoint].startPos]] = currentPoint;
    curPointer.next[str[newLeaf.startPos]] = newLeaf.index;
    
   
    
}

void SuffixTree::UkkonenBuilder::addBranchBetweenVert() {
    Node cuttingVert = Node(tree[currentPoint].startPos, currentOffset, tree[currentPoint].parent, tree.size());
    tree.push_back(cuttingVert);
    ui64 cutVertNumber = tree.size() - 1;
    
    initNewVerticies();
    
    Node& cutPointer = tree[cutVertNumber];
    tree[cutPointer.parent].next[str[tree[cutVertNumber].startPos]] = cutVertNumber;
    
    
    currentLinkPoint = cutVertNumber;
    currentPoint = cutVertNumber;
    firstPos = cutPointer.startPos;
}

inline void SuffixTree::UkkonenBuilder::addBranchFromVert() {
    addLeaf(currentPoint);
    int newLeafIndex = (int)tree.size() - 1;
    
    tree[currentPoint].next[lastChar] = newLeafIndex;
    
    currentPoint = newLeafIndex;
    firstPos = tree[currentPoint].startPos;
}
