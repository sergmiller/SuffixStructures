//
//  ukkonen.cpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include "ukkonen.hpp"

void UkkonenSuffixTree::buildTree(std::string& s) {
    for(size_t i = 0;i < s.size(); ++i) {
        addIntoTree(s[i]);
    }
}

void UkkonenSuffixTree::addIntoTree(char c) {
    ++sizeStr;
    lastChar = c;
    //1 Step: increase leafs from last phase
    ++*lastPos;
    
    //2 Step: calc new nodes in tree
    addNewLeafs();
    
}

void UkkonenSuffixTree::addNewLeafs() {
    size_t numOfCurrentSuffix = numOfLastChangedSuffix + 1;
    
    emptyPhase = false;
    Node* currentSuffix = lastChangedSuffix;
    Node* parent;
    Node* nextSuffParent;
    
    while(!emptyPhase) {
        lastChangedSuffix = currentSuffix;
        parent = lastChangedSuffix->parent;
        lastLiftUp = lastChangedSuffix->length;
        nextSuffParent = parent->link;
        continueTree(nextSuffParent);
    }
    
}

void UkkonenSuffixTree::continueTree(Node* nextSuffParent) {
    if(nextSuffParent->next[lastChar] == nullptr) {
        Node* newLeaf = new Node(sizeStr - 1, 1);
    } else {
        Node* downVert = nextSuffParent->next[lastChar];
        for(
    }
}